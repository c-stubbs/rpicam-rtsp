#include "rpicam_vid_wrapper_config.h"
#include <stdexcept>
#include <array> 
#include <algorithm>
#include <sstream>

RpiCamVidWrapperConfig RpiCamVidWrapperConfig::fromToml(const toml::table& config)
{
    return RpiCamVidWrapperConfig{config["rpicam_vid"]["width"].value_or(1280),
                                  config["rpicam_vid"]["height"].value_or(720),
                                  config["rpicam_vid"]["framerate"].value_or(15),
                                  config["rpicam_vid"]["bitrate"].value_or(1000000),
                                  config["rpicam_vid"]["port"].value_or(5000),
                                  splitString(config["rpicam_vid"]["raw_args"].value_or(""), ' '),
                                  config["rpicam_vid"]["log_level"].value_or("info")};
}

std::vector<std::string> RpiCamVidWrapperConfig::splitString(const std::string& input, const char& delim)
{
    std::vector<std::string> result;
    std::string item;

    std::stringstream ss(input);
    
    while (std::getline(ss, item, delim))
    {
        result.push_back(item);
    }

    return result;
}

RpiCamVidWrapperConfig::RpiCamVidWrapperConfig(int width, int height, int framerate, int bitrate, int port, std::vector<std::string> raw_args, std::string log_level) :
    width(width),
    height(height),
    framerate(framerate),
    bitrate(bitrate),
    port(port),
    raw_args(raw_args),
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
