#include <iostream>
#include <string>
#include <vector>

namespace util {
void strip(std::vector<std::string>&, const std::string&, char);
std::string trim_copy(const std::string&, const std::string& white_space = "");
}  // namespace util
