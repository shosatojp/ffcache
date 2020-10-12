#include "util.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace util {
void split(std::vector<std::string>& result, const std::string& source, char sep) {
    size_t start = 0;
    const size_t str_len = source.size();
    for (size_t i = 0; i < str_len; i++) {
        if (source[i] == sep) {
            result.push_back(source.substr(start, i - start));
            start = i + 1;
        }
    }
    result.push_back(source.substr(start, str_len - start));
}

std::string trim_copy(const std::string& str, const std::string& white_space) {
    if (str.size() == 0) {
        return "";
    } else {
        const size_t start = str.find_first_not_of(white_space);
        const size_t end = str.find_last_not_of(white_space);
        return str.substr(start, end - start + 1);
    }
}
}  // namespace util
