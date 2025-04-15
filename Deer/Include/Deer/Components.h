#pragma once
#include "Deer/Memory.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <stdint.h>

#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace Deer {
	class ComponentScriptInstance;

	struct TagComponent {
		std::string tag;
		uint32_t entityUID;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(std::string name, uint32_t _id = 0)
		    : tag(name), entityUID(_id) {}
	};

	struct ScriptComponent {
		std::string scriptID;
		Ref<ComponentScriptInstance> roeInstance;

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;
		ScriptComponent(std::string _scriptID) : scriptID(_scriptID) {}
	};

	struct RelationshipComponent {
		uint32_t parent_UID = 0;
		std::vector<uint32_t> children;

		RelationshipComponent() = default;
		RelationshipComponent(const RelationshipComponent&) = default;
		RelationshipComponent(uint32_t parent) : parent_UID(parent) {}
	};

	struct TransformComponent {
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(1.0f);
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

		TransformComponent() = default;
		TransformComponent(glm::vec3 _position) : position(_position) {}
		TransformComponent(const TransformComponent&) = default;

		inline const glm::vec3 getEulerAngles() {
			return glm::degrees(glm::eulerAngles(rotation));
		}
		inline void setEulerAngles(const glm::vec3& eulerAngles) {
			rotation = glm::quat(glm::radians(eulerAngles));
		}

		glm::mat4 getMatrix() const;
	};
}  // namespace Deer