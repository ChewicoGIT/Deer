#include "Camera.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "glm/gtc/matrix_transform.hpp"

namespace Deer {
	Camera::Camera(float aspect, float fov, float nearZ, float farZ) 
		: m_aspect(aspect), m_fov(fov), m_nearZ(nearZ), m_farZ(farZ), m_position(glm::vec3(0)), m_rotation(glm::quat()){
		recalculateMatrices();
	}

	void Camera::recalculateMatrices() {
		m_viewMatrix = glm::perspective(glm::radians(m_fov), m_aspect, m_nearZ, m_farZ);
		//m_viewMatrix = glm::inverse(m_viewMatrix);

		glm::mat4 zInvertedMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, -1.0f));
		m_projectionMatrix = glm::translate(glm::mat4(1.0f), m_position);
		m_projectionMatrix = m_projectionMatrix * glm::toMat4(m_rotation);
		m_projectionMatrix = zInvertedMatrix * glm::inverse(m_projectionMatrix);
	}
}
