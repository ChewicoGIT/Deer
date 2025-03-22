#pragma once

#include "Deer/Scene.h"

namespace Deer {
    template<class Archive>
    void serialize(Archive& archive,
        Scene& m_scene) {

        archive(cereal::make_nvp("main_environment", m_scene.getMainEnviroment()));
    }
}