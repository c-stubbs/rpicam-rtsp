#pragma once

#include <toml++/impl/forward_declarations.hpp>
#include <toml++/toml.h>
#include <string>

class RtspServerConfig {

    public:
        static RtspServerConfig fromToml(const toml::v3::ex::parse_result& config);
        RtspServerConfig(int port, int udp_port, std::string mount_point, std::string log_level);

        int port;
        int udp_port;
        std::string mount_point;
        std::string log_level;

        void validate() const;

    private:
        void validatePort() const;
        void validateUdpPort() const;
        void validateMountPoint() const;

};
