#include "ffcache.hpp"

#include <cmath>

#include "util.hpp"

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
