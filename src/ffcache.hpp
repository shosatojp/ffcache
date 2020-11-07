#pragma once
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#include <filesystem>
namespace fs = std::filesystem;

static const int CHUNK_SIZE = 256 * 1024;
static const size_t kHashSize = 20;
typedef uint8_t Hash[kHashSize];
typedef uint64_t OriginAttrsHash;

template <class T>
void endswap(T* objp) {
    unsigned char* memp = reinterpret_cast<unsigned char*>(objp);
    std::reverse(memp, memp + sizeof(T));
}

class HttpHeader {
   public:
    HttpHeader(const std::string& src);
    int status_code;
    std::string status_source;
    std::string status_message;
    std::string protocol;
    std::map<std::string, std::string> headers;
};

struct CacheIndexHeader {
    uint32_t mVersion;
    uint32_t mTimeStamp;
    uint32_t mIsDirty;
    static void read(std::ifstream& ifs, CacheIndexHeader& header);
};

#pragma pack(push, 4)
struct CacheIndexRecord {
    Hash mHash;
    uint32_t mFrecency;
    OriginAttrsHash mOriginAttrsHash;
    uint32_t mExpirationTime;
    uint16_t mOnStartTime;
    uint16_t mOnStopTime;
    uint32_t mFlags;
    std::string hash_tostring() const;
    static void read(std::ifstream& ifs, CacheIndexRecord& record);
};

#pragma pack(pop)
struct FirefoxMetaData {
    uint32_t mVersion;
    uint32_t mFetchCount;
    uint32_t mLastFetched;
    uint32_t mLastModified;
    uint32_t mFrecency;
    uint32_t mExpirationTime;
    uint32_t mKeySize;
    uint32_t mFlags;
    static void read(std::ifstream& ifs, FirefoxMetaData& fmd);
};

class FirefoxCacheEntry {
   public:
    FirefoxCacheEntry(const fs::path& path);
    std::map<std::string, std::string> load_map() const;
    std::unique_ptr<std::vector<char>> get_data() const;
    bool save(const std::string& __path) const;
    HttpHeader get_header() const;

    std::string key;
    std::string file_path;

   private:
    unsigned int meta_start;
    unsigned int meta_end;
    unsigned int map_start;
    FirefoxMetaData metadata;
};

class FirefoxCacheIndex {
   public:
    FirefoxCacheIndex() = default;
    FirefoxCacheIndex(const fs::path& path);
    CacheIndexHeader header;
    std::vector<CacheIndexRecord> records;
};

class FirefoxCache {
   public:
    FirefoxCache(const fs::path& cache2_dir, bool use_index = false);
    FirefoxCacheEntry find(const std::string& key) const;
    FirefoxCacheIndex index;
    std::vector<std::string> keys() const;
    void find_save(const std::string& key,const std::string& path) const;
    std::vector<FirefoxCacheEntry> records;
};
