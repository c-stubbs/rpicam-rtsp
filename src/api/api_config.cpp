#include "api_config.h"

ApiConfig ApiConfig::fromToml(const toml::table& config)
{
    return ApiConfig{config["api"]["run"].value_or(false),
                     config["api"]["log_level"].value_or("info")};
}

ApiConfig::ApiConfig(bool run, std::string log_level)
    : run(run)
    , log_level(log_level)
{
    validate();
}

void ApiConfig::validate() const
{
}
