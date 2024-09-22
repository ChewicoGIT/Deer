#pragma once
#include "Deer/Core/Core.h"
#include "glm/gtc/quaternion.hpp"
#include "Deer/Render/VertexArray.h"
#include "Deer/Render/Shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "entt/entt.hpp"

#include <string>
#include <vector>

namespace Deer {
	struct TagComponent {
		std::string tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(std::string name) : tag(name) { }
	};

	struct RelationshipComponent {
		entt::entity parent_handle = entt::null;
		std::vector<entt::entity> children;
		bool root = false;

		RelationshipComponent() = default;
		RelationshipComponent(const RelationshipComponent&) = default;
		RelationshipComponent(entt::entity parent) : parent_handle(parent) { }
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

	// This requires mesh component to render
	struct MeshRenderComponent {
		MeshRenderComponent() = default;
		MeshRenderComponent(const MeshRenderComponent&) = default;
		MeshRenderComponent(uid _mesh, uid _shader) : shaderAssetID(_shader), meshAssetID(_mesh) { }

		uid shaderAssetID = 0;
		uid meshAssetID = 0;
	};

	// This requires mesh component to render
	struct CameraComponent {
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(float _fov, float _aspect, float _nearZ, float _farZ) : fov(_fov), aspect(_aspect), nearZ(_nearZ), farZ(_farZ) { }

		glm::mat4 getMatrix();

		float fov = glm::radians(50.0f), aspect = 16 / 9, nearZ = 0.1f, farZ = 100;
	};
}