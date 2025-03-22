#pragma once
#include "DeerRender/Components.h"

namespace Deer {
	struct SceneCamera {
		TransformComponent transform;
		CameraComponent camera;

		SceneCamera() {}
		SceneCamera(TransformComponent _transform, CameraComponent _camera) : transform(_transform), camera(_camera) { }
	};
}