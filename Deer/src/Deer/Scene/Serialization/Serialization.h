#pragma once

#include "Deer/Core/Project.h"
#include "Deer/Asset/AssetManager.h"
#include "Deer/Scene/Entity.h"

#include "cereal/cereal.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/string.hpp"

// Serialization Vars
#include "Deer/Scene/Serialization/SerializationGlobalVars.h"

// GENERICS
#include "Deer/Scene/Serialization/Vec3Serialization.h"
#include "Deer/Scene/Serialization/QuatSerialization.h"

// SCENE SPECIFIC
#include "Deer/Scene/Serialization/SceneSerialization.h"
#include "Deer/Scene/Serialization/EnvironmentSerialization.h"
#include "Deer/Scene/Serialization/EntitySerialization.h"

// COMPONENTS SPECIFIC
#include "Deer/Scene/Serialization/Components/TransformComponentSerialization.h"
#include "Deer/Scene/Serialization/Components/RelationshipComponentSerialization.h"
#include "Deer/Scene/Serialization/Components/ScriptComponentSerialization.h"

// RENDER SPECIFIC
#ifdef DEER_RENDER
#include "DeerRender/Scene/Serialization/Components/CameraSerializationComponent.h"
#include "DeerRender/Scene/Serialization/Components/MeshRenderComponentSerialization.h"
#include "DeerRender/Scene/Serialization/Components/TextureBindingSerializationComponent.h"
#endif
