#pragma once
#include "Deer/Core/Core.h"
#include "Deer/Core/Memory.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtc/quaternion.hpp"
#include "glm/glm.hpp"

#ifdef DEER_RENDER
#include "DeerRender/Scene/Components.h"
#endif

#include <string>
#include <vector>

namespace Deer {
	class ComponentScriptInstance;

	struct TagComponent {
		std::string tag;
		uid entityUID;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(std::string name, uid _id = 0) : tag(name), entityUID(_id) { }
	};

	struct ScriptComponent {
		std::string scriptID;
		Ref<ComponentScriptInstance> roeInstance;

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;
		ScriptComponent(std::string _scriptID) : scriptID(_scriptID) { }
	};

	struct RelationshipComponent {
		uid parent_UID = 0;
		std::vector<uid> children;

		RelationshipComponent() = default;
		RelationshipComponent(const RelationshipComponent&) = default;
		RelationshipComponent(uid parent) : parent_UID(parent) { }
	};

	struct TransformComponent {
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(1.0f);
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

		TransformComponent() = default;
		TransformComponent(glm::vec3 _position) : position(_position) { }
		TransformComponent(const TransformComponent&) = default;

		inline const glm::vec3 getEulerAngles() { return glm::degrees(glm::eulerAngles(rotation)); }
		inline void setEulerAngles(const glm::vec3& eulerAngles) { rotation = glm::quat(glm::radians(eulerAngles)); }

		glm::mat4 getMatrix();
	};
}