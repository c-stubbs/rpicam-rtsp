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
        uint8_t fps;
    };

    // Enum could be bool, but leaving opportunity for more states in the fut.
    enum class Status {
        kStopped,
        kRunning
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
            if (json.contains("fps"))
                config.fps = json.at("fps").get<std::uint8_t>();

            return config;
        };

        std::optional<uint16_t> width;
        std::optional<uint16_t> height;
        std::optional<uint8_t> fps;
    };

    struct CameraStatus {
        Status status;
    };

NLOHMANN_JSON_SERIALIZE_ENUM(Status, {
    {Status::kStopped, "stopped"},
    {Status::kRunning, "running"}})

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CameraConfig,
    width, height, fps)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CameraConfigUpdate,
    width, height, fps)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CameraStatus,
    status)

}; // namespace Models
