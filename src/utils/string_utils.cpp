#include <sstream>

#include "string_utils.h"


std::vector<std::string> StringUtils::splitString(const std::string& input, const char& delim) {
    
    std::vector<std::string> result;
    std::string item;

    std::stringstream ss(input);
    
    while (std::getline(ss, item, delim))
    {
        result.push_back(item);
    }

    return result;
}

std::string StringUtils::vectorToString(const std::vector<std::string>& vec) {
    std::ostringstream oss;
    for (const auto& str : vec) {
        oss << str;
    }
    return oss.str();
}
