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
