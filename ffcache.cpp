#include "ffcache.hpp"

#include <cmath>

#include "util.hpp"

//CacheIndexHeader
void CacheIndexHeader::read(std::ifstream* ifs, CacheIndexHeader* header) {
    (*ifs).read((char*)header, sizeof(CacheIndexHeader));
    endswap(&(*header).mVersion);
    endswap(&(*header).mTimeStamp);
    endswap(&(*header).mIsDirty);
}

//CacheIndexRecord
std::string CacheIndexRecord::hash_tostring() {
    std::stringstream str;
    str << hex << uppercase;
    for (size_t i = 0; i < kHashSize; i++)
        str << setfill('0') << right << setw(2) << (int)mHash[i];
    return std::string(str.str());
}
void CacheIndexRecord::read(std::ifstream* ifs, CacheIndexRecord* record) {
    (*ifs).read((char*)record, sizeof(CacheIndexRecord));
    endswap(&(*record).mFrecency);
    endswap(&(*record).mOriginAttrsHash);
    endswap(&(*record).mExpirationTime);
    endswap(&(*record).mOnStartTime);
    endswap(&(*record).mOnStopTime);
    endswap(&(*record).mFlags);
}

//FirefoxMetaData
void FirefoxMetaData::read(std::ifstream* ifs, FirefoxMetaData* fmd) {
    (*ifs).read((char*)fmd, sizeof(FirefoxMetaData));
    endswap(&(*fmd).mVersion);
    endswap(&(*fmd).mExpirationTime);
    endswap(&(*fmd).mFetchCount);
    endswap(&(*fmd).mFlags);
    endswap(&(*fmd).mFrecency);
    endswap(&(*fmd).mKeySize);
    endswap(&(*fmd).mLastFetched);
    endswap(&(*fmd).mLastModified);
}

//FirefoxCacheEntry
FirefoxCacheEntry::FirefoxCacheEntry(std::string path) {
    this->file_path = path;
    std::ifstream ifs(path, std::ios::binary);
    //meta start
    ifs.seekg(-4, std::ios::end);
    this->meta_end = ifs.tellg();
    ifs.read((char*)&meta_start, 4);
    endswap(&meta_start);
    //meta data
    int numHashChunks = (int)std::ceil((double)meta_start / CHUNK_SIZE);
    ifs.seekg(meta_start + 4 + numHashChunks * 2, std::ios::beg);
    FirefoxMetaData::read(&ifs, &metadata);
    //key
    char* key = new char[metadata.mKeySize + 1];
    ifs.read(key, metadata.mKeySize + 1);
    std::string keysrc(key, metadata.mKeySize);
    this->key = keysrc.substr(keysrc.find(':') + 1);
    delete[] key;
    ifs.seekg(1, std::ios::cur);
    this->map_start = ifs.tellg();
    ifs.close();
}
std::map<std::string, std::string> FirefoxCacheEntry::load_map() {
    std::map<std::string, std::string> result;
    std::ifstream ifs(this->file_path, std::ios::binary);
    ifs.seekg(map_start, std::ios::beg);
    std::vector<std::string> vec;
    std::stringstream ss;
    char c;
    while (ifs.read(&c, 1), ifs.tellg() < this->meta_end) {
        if (c != '\0')
            ss << c;
        else {
            vec.push_back(ss.str());
            ss.str("");
            ss.clear();
        }
    }
    ifs.close();
    for (size_t i = 0; i + 1 < vec.size(); i += 2) {
        result[vec[i]] = vec[i + 1];
    }
    return result;
}
void FirefoxCacheEntry::get_data(char** data, int* size) {
    std::ifstream ifs(this->file_path, std::ios::binary);
    char* data_ = new char[this->meta_start];
    ifs.read(data_, this->meta_start);
    ifs.close();
    *size = this->meta_start;
    *data = data_;
}
void FirefoxCacheEntry::save(std::string path) {
    int size;
    char* data = nullptr;
    this->get_data(&data, &size);
    ofstream ofs(path, std::ios::binary);
    ofs.write(data, size);
    ofs.close();
    delete[] data;
}
HttpHeader FirefoxCacheEntry::get_header() {
    return HttpHeader(this->load_map()["response-head"]);
}
//FirefoxCacheIndex
FirefoxCacheIndex::FirefoxCacheIndex(std::string path) {
    std::ifstream ifs(path, std::ios::binary);
    CacheIndexHeader::read(&ifs, &header);
    while (!ifs.eof()) {
        CacheIndexRecord cir;
        CacheIndexRecord::read(&ifs, &cir);
        records.push_back(cir);
    }
}

//HttpHeader
HttpHeader::HttpHeader(std::string src) {
    std::vector<std::string> result;
    strip(result, src, '\n');
    //status
    status_source = trim_copy(result.front());
    std::vector<std::string> status_temp;
    strip(status_temp, status_source, ' ');
    if (status_temp.size() == 3) {
        this->protocol = status_temp[0];
        this->status_code = stoi(status_temp[1]);
        this->status_message = status_temp[2];
    }
    result.erase(result.begin());
    //headers
    for (std::string s : result) {
        std::vector<std::string> header;
        strip(header, s, ':');
        if (header.size() == 2) {
            headers[trim_copy(header[0])] = trim_copy(header[1]);
        }
    }
}

//FirefoxCache
FirefoxCache::FirefoxCache(fs::path cache2_dir, bool use_index) {
    fs::path index_file_path = cache2_dir / "index";
    fs::path cache_entry_dir = cache2_dir / "entries";
    if (use_index) {
        FirefoxCacheIndex index(index_file_path.string());
        for (int i = 0, length = index.records.size() - 1; i < length; i++) {
            FirefoxCacheEntry ff((cache_entry_dir / index.records[i].hash_tostring()).string());
            records.push_back(ff);
        }
    } else {
        for (const auto& entry : fs::directory_iterator(cache_entry_dir)) {
            records.push_back(FirefoxCacheEntry(entry.path().string()));
        }
    }
}
FirefoxCacheEntry FirefoxCache::find(std::string key) {
    for (FirefoxCacheEntry e : records) {
        if (e.key == key) return e;
    }
    throw std::exception();
}
void FirefoxCache::find_save(std::string key, std::string path) {
    FirefoxCacheEntry ff = this->find(key);
    ff.save(path);
}
std::vector<std::string> FirefoxCache::keys() {
    std::vector<std::string> keys;
    for (FirefoxCacheEntry ff : this->records) {
        keys.push_back(ff.key);
    }
    return keys;
}