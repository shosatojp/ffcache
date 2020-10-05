
#include <cmath>
#include <gzip/decompress.hpp>
#include "ffcache.hpp"
#include "util.hpp"

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

void FirefoxCacheEntry::save(std::string path, bool decompress_gzip) {
    int size;
    char* data = nullptr;
    this->get_data(&data, &size);
    ofstream ofs(path, std::ios::binary);
    if (this->get_header().is_gzipped()) {
        std::cout << "gzip" << std::endl;
        ofs << gzip::decompress(data, size);
    } else {
        ofs.write(data, size);
    }
    ofs.close();
    delete[] data;
}

HttpHeader FirefoxCacheEntry::get_header() {
    return HttpHeader(this->load_map()["response-head"]);
}
