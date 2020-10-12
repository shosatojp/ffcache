#include "ffcache.hpp"

#include <cmath>

#include "util.hpp"

//FirefoxCache
FirefoxCache::FirefoxCache(const fs::path& cache2_dir, bool use_index) {
    const fs::path index_file_path = cache2_dir / "index";
    const fs::path cache_entry_dir = cache2_dir / "entries";
    if (use_index) {
        const FirefoxCacheIndex index{index_file_path};
        for (auto& record : index.records) {
            const fs::path path = cache_entry_dir / record.hash_tostring();
            if (fs::exists(path)) {
                try {
                    records.push_back(FirefoxCacheEntry(path));
                } catch (const std::exception& e) {
                    std::cerr << e.what() << '\n';
                }
            }
        }
    } else {
        for (const auto& entry : fs::directory_iterator(cache_entry_dir)) {
            try {
                records.push_back(FirefoxCacheEntry(entry.path()));
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
        }
    }
}

FirefoxCacheEntry FirefoxCache::find(const std::string& key) const {
    for (const FirefoxCacheEntry& e : records) {
        if (e.key == key) return e;
    }
    throw std::exception();
}

void FirefoxCache::find_save(const std::string& key,const std::string& path) const {
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
