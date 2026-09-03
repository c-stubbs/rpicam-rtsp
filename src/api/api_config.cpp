#include "api_config.h"

ApiConfig ApiConfig::fromToml(const toml::v3::ex::parse_result& config)
{
    return ApiConfig{config["api"]["log_level"].value_or("info")};
}

ApiConfig::ApiConfig(std::string log_level)
    : log_level(log_level)
{
    validate();
}

void ApiConfig::validate() const
{
}
