#pragma once
#include "DeerRender/Scene/Components.h"

namespace Deer {
    // CAMERA COMPONENT
    template<class Archive>
    void serialize(Archive& archive,
        CameraComponent& camera) {

        archive(cereal::make_nvp("aspect", camera.aspect));
        archive(cereal::make_nvp("fov", camera.fov));
        archive(cereal::make_nvp("farZ", camera.farZ));
        archive(cereal::make_nvp("nearZ", camera.nearZ));
    }
}