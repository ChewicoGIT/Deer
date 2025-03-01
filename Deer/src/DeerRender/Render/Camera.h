#pragma once
#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtc/quaternion.hpp"

namespace Deer {
	class Camera {
	public:
		Camera(float aspect, float fov = 60, float nearZ = 0.1f, float farZ = 500);

		void setPosition(const glm::vec3& position) { m_position = position; }
		const glm::vec3& getPosition() { return m_position; }

		void setRotation(const glm::quat& rotation) { m_rotation = rotation; }
		const glm::quat& getRotation() { return m_rotation; }

		void setAspect(float aspect) { m_aspect = aspect; }
		float getAspect() { return m_aspect; }

		void setFov(float fov) { m_fov = fov; }
		float getFov() { return m_fov; }

		void recalculateMatrices();
		const glm::mat4& getProjectionMatrix() { return m_projectionMatrix; }
		const glm::mat4& getViewMatrix() { return m_viewMatrix; }
	private:
		float m_fov, m_aspect, m_nearZ, m_farZ;
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;

		glm::vec3 m_position;
		glm::quat m_rotation;
	};
}

