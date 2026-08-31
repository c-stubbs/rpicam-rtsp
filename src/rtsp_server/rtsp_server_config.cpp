#include "rtsp_server_config.h"
#include <stdexcept>

RtspServerConfig RtspServerConfig::fromToml(const toml::v3::ex::parse_result& config)
{
    return RtspServerConfig{config["rtsp_server"]["port"].value_or(8554),
                            config["rtsp_server"]["port"].value_or("stream")};
}

RtspServerConfig::RtspServerConfig(int port, std::string endpoint)
{
    validate();
}

void RtspServerConfig::validate() const
{
    validatePort();
    validateEndpoint();
}

void RtspServerConfig::validatePort() const
{
    if (port <= 0 || port > 65535)
    {
        throw std::runtime_error("Invalid port " + std::to_string(port));
    }
}

void RtspServerConfig::validateEndpoint() const
{
    if (endpoint.empty())
    {
        throw std::runtime_error("Endpoint cannot be empty");
    }
}
