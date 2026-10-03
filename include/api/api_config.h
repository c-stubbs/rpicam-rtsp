#pragma once

#include <toml++/toml.h>

class ApiConfig {

    public:
        static ApiConfig fromToml(const toml::table& config);
        ApiConfig(std::string log_level);

        void validate() const;

        std::string log_level;

    private:

};
