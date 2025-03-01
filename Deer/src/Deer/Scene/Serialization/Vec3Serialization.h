#pragma once
#include "glm/glm.hpp"

namespace cereal {
    template<class Archive>
    void serialize(Archive& archive,
        glm::vec3& vec3) {

        archive(
            cereal::make_nvp("x", vec3.x),
            cereal::make_nvp("y", vec3.y),
            cereal::make_nvp("z", vec3.z));
    }
}