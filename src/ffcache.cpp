#include "ffcache.hpp"

#include <cmath>

#include "util.hpp"

//FirefoxCache
FirefoxCache::FirefoxCache(fs::path cache2_dir, bool use_index) {
    fs::path index_file_path = cache2_dir / "index";
    fs::path cache_entry_dir = cache2_dir / "entries";
    if (use_index) {
        FirefoxCacheIndex index(index_file_path.string());
        for (auto& record : index.records) {
            fs::path path = cache_entry_dir / record.hash_tostring();
            records.push_back(FirefoxCacheEntry(path));
        }
    } else {
        for (const auto& entry : fs::directory_iterator(cache_entry_dir)) {
            records.push_back(FirefoxCacheEntry(entry.path()));
        }
    }
}

FirefoxCacheEntry FirefoxCache::find(std::string key) const {
    for (const FirefoxCacheEntry& e : records) {
        if (e.key == key) return e;
    }
    throw std::exception();
}

void FirefoxCache::find_save(std::string key, std::string path) const {
    const FirefoxCacheEntry& ff = this->find(key);
    ff.save(path);
}

std::vector<std::string> FirefoxCache::keys() const {
    std::vector<std::string> keys;
    for (const FirefoxCacheEntry& ff : this->records) {
        keys.push_back(ff.key);
    }
    return keys;
}
