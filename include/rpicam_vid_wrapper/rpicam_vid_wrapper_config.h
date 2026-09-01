#pragma once

#include <toml++/toml.h>
#include <string>

class RpiCamVidWrapperConfig {

    public:
        static RpiCamVidWrapperConfig fromToml(const toml::v3::ex::parse_result& config);
        RpiCamVidWrapperConfig(int width, int height, int framerate, int bitrate, int port, std::string log_level);
        
        int width;
        int height;
        int framerate;
        int bitrate;
        int port;
        std::string log_level;

        void validate() const;

    private:
        void validateResolution() const;
        void validateFramerate() const;
        void validateBitrate() const;
        void validatePort() const;
            
};
