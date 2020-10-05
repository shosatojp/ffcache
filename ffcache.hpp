#pragma once
#include <stdio.h>

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

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

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
    HttpHeader(std::string src);
    int status_code;
    std::string status_source;
    std::string status_message;
    std::string protocol;
    std::map<std::string, std::string> headers;
    bool is_gzipped();
};

struct CacheIndexHeader {
    uint32_t mVersion;
    uint32_t mTimeStamp;
    uint32_t mIsDirty;
    static void read(std::ifstream* ifs, CacheIndexHeader* header);
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
    std::string hash_tostring();
    static void read(std::ifstream* ifs, CacheIndexRecord* record);
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
    static void read(std::ifstream* ifs, FirefoxMetaData* fmd);
};

class FirefoxCacheEntry {
   public:
    FirefoxCacheEntry(std::string path);
    std::map<std::string, std::string> load_map();
    void get_data(char** data, int* size);
    void save(std::string path, bool decompress_gzip = true);
    HttpHeader get_header();

    std::string file_path;
    int meta_start;
    int meta_end;
    int map_start;
    FirefoxMetaData metadata;
    std::string key;
};

class FirefoxCacheIndex {
   public:
    FirefoxCacheIndex(){};
    FirefoxCacheIndex(std::string path);
    CacheIndexHeader header;
    std::vector<CacheIndexRecord> records;
};

class FirefoxCache {
   public:
    FirefoxCache(std::string cache2_dir, bool use_index = false)
        : FirefoxCache(fs::path(cache2_dir), use_index) {}
    FirefoxCache(fs::path cache2_dir, bool use_index = false);
    FirefoxCacheEntry find(std::string key);
    FirefoxCacheIndex index;
    std::vector<std::string> keys();
    void find_save(std::string key, std::string path);
    std::vector<FirefoxCacheEntry> records;
};
