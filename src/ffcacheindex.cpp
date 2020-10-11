#include "ffcache.hpp"

#include <cmath>

#include "util.hpp"

//FirefoxCacheIndex
FirefoxCacheIndex::FirefoxCacheIndex(const std::string& path) {
    std::ifstream ifs(path, std::ios::binary);
    CacheIndexHeader::read(ifs, header);
    while (!ifs.eof()) {
        CacheIndexRecord cir;
        CacheIndexRecord::read(ifs, cir);
        records.push_back(std::move(cir));
    }
}
