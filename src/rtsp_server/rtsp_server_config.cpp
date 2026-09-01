#include "rtsp_server_config.h"
#include <stdexcept>

RtspServerConfig RtspServerConfig::fromToml(const toml::v3::ex::parse_result& config)
{
    return RtspServerConfig{config["rtsp_server"]["port"].value_or(8554),
                            config["rtsp_server"]["udp_port"].value_or(5000),
                            config["rtsp_server"]["mount_point"].value_or("stream"),
                            config["rtsp_server"]["log_level"].value_or("info")};
}

RtspServerConfig::RtspServerConfig(int port, int udp_port, std::string mount_point, std::string log_level)
    : port(port)
    , udp_port(udp_port)
    , mount_point(mount_point)
    , log_level(log_level)
{
    validate();
}

void RtspServerConfig::validate() const
{
    validatePort();
    validateUdpPort();
    validateMountPoint();
}

void RtspServerConfig::validatePort() const
{
    if (port <= 0 || port > 65535)
    {
        throw std::runtime_error("Invalid port " + std::to_string(port));
    }
}

void RtspServerConfig::validateUdpPort() const
{
    if (udp_port <= 0 || udp_port > 65535)
    {
        throw std::runtime_error("Invalid udp port " + std::to_string(udp_port));
    }
}

void RtspServerConfig::validateMountPoint() const
{
    if (mount_point.empty())
    {
        throw std::runtime_error("Endpoint cannot be empty");
    }
}
