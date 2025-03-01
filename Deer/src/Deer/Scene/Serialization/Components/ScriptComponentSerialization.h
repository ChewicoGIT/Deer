#pragma once
#include "Deer/Scene/Components.h"

namespace Deer {
    // SCRIPT COMPONENT
    template<class Archive>
    void serialize(Archive& archive,
        ScriptComponent& scriptComponent) {

        archive(cereal::make_nvp("scriptID", scriptComponent.scriptID));
    }
}