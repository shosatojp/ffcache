#include <iostream>
#include <string>

#include "ffcache.hpp"

int main(int argc, const char* argv[]) {
    if (argc > 3) {
        // argv[1] : cache dir
        // argv[2] : key
        // argv[3] : output path
        try {
            fs::path dir(argv[1]);
            FirefoxCache index(dir);
            FirefoxCacheEntry ff = index.find(argv[2]);
            ff.save(argv[3]);
            exit(0);
        } catch (...) {
            std::cout << "not found" << std::endl;
            exit(2);
        }
    } else if (argc > 1) {
        fs::path dir(argv[1]);
        FirefoxCache index(dir);

        std::string buf;
        while (std::getline(std::cin, buf)) {
            std::string key, path;
            try {
                auto sep = std::find(buf.begin(), buf.end(), ' ');
                key = buf.substr(0, sep - buf.begin());
                path = buf.substr(sep - buf.begin() + 1, buf.size());
            } catch (...) {
                std::cout << "invalid input" << std::endl;
                exit(2);
            }

            try {
                FirefoxCacheEntry ff = index.find(key);
                ff.save(path);
                std::cout << path << std::endl;
            } catch (const std::exception& e) {
                std::cout << "not found" << std::endl;
            }
        }

    } else {
        std::cout << "not enough arguments" << std::endl
                  << "[usage]" << std::endl
                  << "ffcache <cache-dir> <url> <output-file>" << std::endl;
        exit(1);
    }
}