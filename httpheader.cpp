
#include <cmath>

#include "ffcache.hpp"
#include "util.hpp"

//HttpHeader
HttpHeader::HttpHeader(std::string src) {
    std::vector<std::string> result;
    util::strip(result, src, '\n');
    //status
    status_source = util::trim_copy(result.front());
    std::vector<std::string> status_temp;
    util::strip(status_temp, status_source, ' ');
    if (status_temp.size() == 3) {
        this->protocol = status_temp[0];
        this->status_code = stoi(status_temp[1]);
        this->status_message = status_temp[2];
    }
    result.erase(result.begin());
    //headers
    for (std::string s : result) {
        std::vector<std::string> header;
        util::strip(header, s, ':');
        if (header.size() == 2) {
            std::cout << header[0] << header[1] << std::endl;
            headers[util::trim_copy(header[0])] = util::trim_copy(header[1]);
        }
    }
}