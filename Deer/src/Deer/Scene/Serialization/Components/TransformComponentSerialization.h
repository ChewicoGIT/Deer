#pragma once
#include "Deer/Scene/Components.h"

namespace Deer {

    // TRANSFORM COMPONENT
    template<class Archive>
    void serialize(Archive& archive,
        TransformComponent& transform) {

        archive(cereal::make_nvp("position", transform.position));
        archive(cereal::make_nvp("scale", transform.scale));
        archive(cereal::make_nvp("rotation", transform.rotation));
    }

}