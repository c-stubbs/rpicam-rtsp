#pragma once

#include <toml++/toml.h>

class RpiCamVidWrapperConfig {

    public:
        static RpiCamVidWrapperConfig fromToml(const toml::v3::ex::parse_result& config);
        RpiCamVidWrapperConfig(int width, int height, int framerate, int bitrate, int port);
        
        int width;
        int height;
        int framerate;
        int bitrate;
        int port;

        void validate() const;

    private:
        void validateResolution() const;
        void validateFramerate() const;
        void validateBitrate() const;
        void validatePort() const;
            
};
