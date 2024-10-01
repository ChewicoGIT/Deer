#pragma once
#include "Deer/Core/Core.h"
#include "Deer/Render/VertexArray.h"
#include "Deer/Render/Shader.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtc/quaternion.hpp"
#include "glm/glm.hpp"

#include <string>
#include <vector>

#define MAX_TEXTURE_BINDINGS 4

namespace Deer {
	struct TagComponent {
		std::string tag;
		uid entityUID;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(std::string name, uid _id = 0) : tag(name), entityUID(_id) { }
	};

	struct RelationshipComponent {
		uid parent_UID = 0;
		std::vector<uid> children;

		RelationshipComponent() = default;
		RelationshipComponent(const RelationshipComponent&) = default;
		RelationshipComponent(uid parent) : parent_UID(parent) { }
	};

	struct TransformComponent {
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;

		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(1.0f);
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

		inline const glm::vec3& getEulerAngles() { return glm::degrees(glm::eulerAngles(rotation)); }
		inline void setEulerAngles(const glm::vec3& eulerAngles) { rotation = glm::quat(glm::radians(eulerAngles)); }

		glm::mat4 getMatrix();
	};

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

		glm::mat4 getMatrix();

		float fov = glm::radians(50.0f), aspect = 16 / 9, nearZ = 0.1f, farZ = 100;
	};
}