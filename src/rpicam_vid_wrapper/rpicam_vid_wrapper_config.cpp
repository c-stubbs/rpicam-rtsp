#include "rpicam_vid_wrapper_config.h"
#include <stdexcept>
#include <array> 
#include <algorithm>

RpiCamVidWrapperConfig RpiCamVidWrapperConfig::fromToml(const toml::v3::ex::parse_result& config)
{
    return RpiCamVidWrapperConfig{config["rpicam_vid"]["width"].value_or(1280),
                                  config["rpicam_vid"]["height"].value_or(720),
                                  config["rpicam_vid"]["framerate"].value_or(15),
                                  config["rpicam_vid"]["bitrate"].value_or(1000000),
                                  config["rpicam_vid"]["port"].value_or(5000),
                                  config["rpicam_vid"]["log_level"].value_or("info")};
}

RpiCamVidWrapperConfig::RpiCamVidWrapperConfig(int width, int height, int framerate, int bitrate, int port, std::string log_level) :
    width(width),
    height(height),
    framerate(framerate),
    bitrate(bitrate),
    port(port),
    log_level(log_level)
{
    validate();
}

void RpiCamVidWrapperConfig::validate() const
{
    validateResolution();
    validateFramerate();
    validateBitrate();
    validatePort();
}

void RpiCamVidWrapperConfig::validateResolution() const
{
    constexpr std::array valid_resolutions = {
        std::pair{620, 480},
        std::pair{1280, 720},
        std::pair{1920, 1080}
    };

    const auto resolution = std::pair{width, height};

    if (std::find(
                valid_resolutions.begin(),
                valid_resolutions.end(),
                resolution) == valid_resolutions.end())
    {
        throw std::runtime_error(
                "Invalid camera resolution: " +
                std::to_string(width) + "x" +
                std::to_string(height));
    }
}

void RpiCamVidWrapperConfig::validateFramerate() const
{
    // TODO: valid framerates change depending on resolution
    constexpr std::array<int, 3> valid_framerates = {
        15,
        30,
        45
    };

    if (std::find(
                valid_framerates.begin(),
                valid_framerates.end(),
                framerate) == valid_framerates.end())
    {
        throw std::runtime_error("Invalid framerate: " + std::to_string(framerate));
    }
}

void RpiCamVidWrapperConfig::validateBitrate() const
{
    if (bitrate <= 0 || bitrate > 12000000)
    {
        throw std::runtime_error("Invalid bitrate: " + std::to_string(bitrate));
    }
}

void RpiCamVidWrapperConfig::validatePort() const
{
    if (port <= 0 || port > 65535)
    {
        throw std::runtime_error("Invalid port: " + std::to_string(port));
    }
}
