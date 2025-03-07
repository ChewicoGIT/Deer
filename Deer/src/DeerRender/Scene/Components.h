#pragma once

#include "DeerRender/Render/VertexArray.h"
#include "DeerRender/Render/Shader.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtc/quaternion.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define MAX_TEXTURE_BINDINGS 4

namespace Deer {
	struct MeshRenderComponent {
		MeshRenderComponent() = default;
		MeshRenderComponent(const MeshRenderComponent&) = default;
		MeshRenderComponent(uid _mesh, uid _shader) : shaderAssetID(_shader), meshAssetID(_mesh) { }

		uid shaderAssetID = 0;
		uid meshAssetID = 0;
	};

	struct TextureBindingComponent {
		TextureBindingComponent() {
			for (int x = 0; x < MAX_TEXTURE_BINDINGS; x++) {
				textureAssetID[x] = 0;
				textureBindID[x] = 0;
			}
		}
		TextureBindingComponent(const TextureBindingComponent&) = default;

		uid textureAssetID[MAX_TEXTURE_BINDINGS];
		unsigned char textureBindID[MAX_TEXTURE_BINDINGS];
	};

	struct CameraComponent {
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(float _fov, float _aspect, float _nearZ, float _farZ) : fov(_fov), aspect(_aspect), nearZ(_nearZ), farZ(_farZ) { }

		inline glm::mat4 getMatrix() { return glm::perspective(fov, aspect, nearZ, farZ); }

		float fov = glm::radians(50.0f), aspect = 16 / 9, nearZ = 0.1f, farZ = 1000;
	};

}