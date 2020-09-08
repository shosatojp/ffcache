#include "util.hpp"

#include <iostream>
#include <string>
#include <vector>

void strip(std::vector<std::string>& v, const std::string& str, char sep) {
    size_t start = 0;
    size_t str_len = str.size();
    for (size_t i = 0; i < str_len; i++) {
        if (str[i] == sep) {
            v.push_back(str.substr(start, i));
            start = i + 1;
        }
    }
    v.push_back(str.substr(start, str_len - 1));
}

std::string trim_copy(const std::string& str, const std::string& white_space) {
    if (str.size() == 0) {
        return "";
    } else {
        size_t start = str.find_first_not_of(white_space);
        size_t end = str.find_last_not_of(white_space);
        return str.substr(start, end - 1);
    }
}
