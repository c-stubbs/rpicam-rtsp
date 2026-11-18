#pragma once

#include <string>
#include <vector>

namespace StringUtils{

    std::vector<std::string> splitString(const std::string& input, const char& delim);
    std::string vectorToString(const std::vector<std::string>& vec);

}; // namespace StringUtils
