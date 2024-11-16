#pragma once
#include "Deer/Scene/Components.h"

namespace Deer {
	struct SceneCamera {
		TransformComponent transform;
		CameraComponent camera;
	};
}