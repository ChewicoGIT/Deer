#pragma once"Deer/Scene/ComponentsRender.h"
#include "Deer/Scene/Components.h"
#include "DeerRender/Scene/ComponentsRender.h"

namespace Deer {
	struct SceneCamera {
		TransformComponent transform;
		CameraComponent camera;
	};
}