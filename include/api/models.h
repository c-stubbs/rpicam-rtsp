#pragma once

#include <cstdint>
#include <optional>

#include "nlohmann/json.hpp"

namespace Models {

    template <typename T>
    struct Model
    {
        std::string dump()
        {
            return nlohmann::json(static_cast<const T&>(*this)).dump();
        }
    };

    struct CameraConfig : Model<CameraConfig>
    {
        uint16_t width;
        uint16_t height;
        uint8_t framerate;
        uint32_t bitrate;
        uint16_t port;
        std::string log_level;
        std::string raw_args;
    };

    struct CameraConfigUpdate {

        // nlohmann doesn't seem to handle optionals how I want so DIY
        static CameraConfigUpdate fromJson(const nlohmann::json& json)
        {
            CameraConfigUpdate config;
            if (json.contains("width"))
                config.width = json.at("width").get<std::uint16_t>();
            if (json.contains("height"))
                config.height = json.at("height").get<std::uint16_t>();
            if (json.contains("framerate"))
                config.framerate = json.at("framerate").get<std::uint8_t>();
            if (json.contains("bitrate"))
                config.bitrate = json.at("bitrate").get<std::uint32_t>();
            if (json.contains("port"))
                config.port = json.at("port").get<std::uint16_t>();
            if (json.contains("log_level"))
                config.log_level = json.at("log_level").get<std::string>();
            if (json.contains("raw_args"))
                config.raw_args = json.at("raw_args").get<std::string>();

            return config;
        };

        std::optional<uint16_t> width;
        std::optional<uint16_t> height;
        std::optional<uint8_t> framerate;
        std::optional<uint32_t> bitrate;
        std::optional<uint16_t> port;
        std::optional<std::string> log_level;
        std::optional<std::string> raw_args;
    };


    // Enum could be bool, but leaving opportunity for more states in the fut.
    enum class Status {
        kStopped,
        kRunning
    };

    struct CameraStatus {
        Status status;
    };

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CameraConfig,
    width, height, framerate)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CameraConfigUpdate,
    width, height, framerate)

NLOHMANN_JSON_SERIALIZE_ENUM(Status, {
    {Status::kStopped, "stopped"},
    {Status::kRunning, "running"}})

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CameraStatus,
    status)

}; // namespace Models
