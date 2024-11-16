#pragma once
#include "Deer/Core/Core.h"
#include "Deer/Scene/Scene.h"

namespace Deer {
    template<class Archive>
    void serialize(Archive& archive,
        Ref<Scene>& m_scene) {

        archive(cereal::make_nvp("main_environment", m_scene->getMainEnviroment()));
    }
}