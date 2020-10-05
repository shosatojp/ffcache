
#include <cmath>

#include "ffcache.hpp"
#include "util.hpp"

//HttpHeader
HttpHeader::HttpHeader(std::string src) {
    std::vector<std::string> result;
    util::split(result, src, '\n');
    //status
    status_source = util::trim_copy(result.front());
    std::vector<std::string> status_temp;
    util::split(status_temp, status_source, ' ');
    if (status_temp.size() == 3) {
        this->protocol = status_temp[0];
        this->status_code = stoi(status_temp[1]);
        this->status_message = status_temp[2];
    }
    result.erase(result.begin());
    //headers
    for (std::string s : result) {
        std::vector<std::string> header;
        util::split(header, s, ':');
        if (header.size() == 2) {
            std::string&& key = util::trim_copy(header[0], " \r\n");
            std::transform(key.begin(), key.end(), key.begin(), [](char c) {
                return std::tolower(c);
            });
            headers[key] = util::trim_copy(header[1], " \r\n");
        }
    }
}

bool HttpHeader::is_gzipped() {
    return headers.count("content-encoding") != 0 && headers["content-encoding"] == "gzip";
}