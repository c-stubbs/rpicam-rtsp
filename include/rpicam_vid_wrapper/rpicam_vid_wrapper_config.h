#pragma once

#include <toml++/toml.h>
#include <string>

class RpiCamVidWrapperConfig {

    public:
        static RpiCamVidWrapperConfig fromToml(const toml::table& config);
        RpiCamVidWrapperConfig(int width, int height, int framerate, int bitrate, int port, std::vector<std::string> raw_args, std::string log_level);
        
        int width;
        int height;
        int framerate;
        int bitrate;
        int port;
        std::string log_level;
        std::vector<std::string> raw_args;

        void validate() const;

    private:
        static std::vector<std::string> splitString(const std::string& input, const char& delim);

        void validateResolution() const;
        void validateFramerate() const;
        void validateBitrate() const;
        void validatePort() const;

};
