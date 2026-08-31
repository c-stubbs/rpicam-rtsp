#pragma once

#include <toml++/impl/forward_declarations.hpp>
#include <toml++/toml.h>
#include <string>

class RtspServerConfig {

    public:
        static RtspServerConfig fromToml(const toml::v3::ex::parse_result& config);
        RtspServerConfig(int port , std::string config);

        int port;
        std::string endpoint;

        void validate() const;

    private:
        void validatePort() const;
        void validateEndpoint() const;

};
