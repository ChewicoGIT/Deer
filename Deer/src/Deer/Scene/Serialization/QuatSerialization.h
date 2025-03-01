#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtc/quaternion.hpp"

namespace cereal {

    template<class Archive>
    void serialize(Archive& archive,
        glm::quat& rot) {

        archive(cereal::make_nvp("x", rot.x));
        archive(cereal::make_nvp("y", rot.y));
        archive(cereal::make_nvp("z", rot.z));
        archive(cereal::make_nvp("w", rot.w));
    }
}