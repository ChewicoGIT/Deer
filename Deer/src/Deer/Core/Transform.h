#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace Deer {
	struct Transform {
		Transform()
			: position(glm::vec3()), scale(glm::vec3(1.0f)), rotation(glm::quat()) { }

		glm::vec3 position;
		glm::vec3 scale;
		glm::quat rotation;

		inline const glm::vec3& getEulerAngles() { return glm::eulerAngles(rotation); }
		inline void setEulerAngles(const glm::vec3& eulerAngles) { rotation = glm::quat(eulerAngles); }

		glm::mat4 getWorldMatrix();
	};
}

