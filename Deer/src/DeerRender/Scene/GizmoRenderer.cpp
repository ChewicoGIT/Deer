#include "GizmoRenderer.h"
#include "DeerRender/Render/Render.h"
#include "DeerRender/Render/RenderUtils.h"
#include "Deer/Scene/Components.h"

namespace Deer {
	void GizmoRenderer::drawLine(glm::vec3 a, glm::vec3 b, glm::vec3 color) {
		m_lines.push_back({ a, b, color });
	}

	void GizmoRenderer::drawVoxelLine(int _x, int _y, int _z, glm::vec3 color) {
		for (int x = 0; x < 2; x++) {
			for (int y = 0; y < 2; y++) {
				glm::vec3 a = glm::vec3(x + _x, y + _y, 0 + _z);
				glm::vec3 b = glm::vec3(x + _x, y + _y, 1 + _z);

				drawLine(a, b, color);
			}
		}
		for (int z = 0; z < 2; z++) {
			for (int y = 0; y < 2; y++) {
				glm::vec3 a = glm::vec3(0 + _x, y + _y, z + _z);
				glm::vec3 b = glm::vec3(1 + _x, y + _y, z + _z);

				drawLine(a, b, color);
			}
		}
		for (int x = 0; x < 2; x++) {
			for (int z = 0; z < 2; z++) {
				glm::vec3 a = glm::vec3(x + _x, 0 + _y, z + _z);
				glm::vec3 b = glm::vec3(x + _x, 1 + _y, z + _z);

				drawLine(a, b, color);
			}
		}

	}

	void GizmoRenderer::drawVoxelLineFace(int x, int y, int z, uint8_t face, glm::vec3 color) {
		glm::vec3 points[4];

		switch (face)
		{
		case 0:
			points[0] = glm::vec3(x + 0, y + 0, z + 0);
			points[1] = glm::vec3(x + 0, y + 0, z + 1);
			points[2] = glm::vec3(x + 0, y + 1, z + 0);
			points[3] = glm::vec3(x + 0, y + 1, z + 1);
			break;
		case 1:
			points[0] = glm::vec3(x + 1, y + 0, z + 0);
			points[1] = glm::vec3(x + 1, y + 0, z + 1);
			points[2] = glm::vec3(x + 1, y + 1, z + 0);
			points[3] = glm::vec3(x + 1, y + 1, z + 1);
			break;
		case 2:
			points[0] = glm::vec3(x + 0, y + 0, z + 0);
			points[1] = glm::vec3(x + 1, y + 0, z + 0);
			points[2] = glm::vec3(x + 0, y + 0, z + 1);
			points[3] = glm::vec3(x + 1, y + 0, z + 1);
			break;
		case 3:
			points[0] = glm::vec3(x + 0, y + 1, z + 0);
			points[1] = glm::vec3(x + 1, y + 1, z + 0);
			points[2] = glm::vec3(x + 0, y + 1, z + 1);
			points[3] = glm::vec3(x + 1, y + 1, z + 1);
			break;
		case 4:
			points[0] = glm::vec3(x + 0, y + 0, z + 0);
			points[1] = glm::vec3(x + 1, y + 0, z + 0);
			points[2] = glm::vec3(x + 0, y + 1, z + 0);
			points[3] = glm::vec3(x + 1, y + 1, z + 0);
			break;
		case 5:
			points[0] = glm::vec3(x + 0, y + 0, z + 1);
			points[1] = glm::vec3(x + 1, y + 0, z + 1);
			points[2] = glm::vec3(x + 0, y + 1, z + 1);
			points[3] = glm::vec3(x + 1, y + 1, z + 1);
			break;
		default:
			points[0] = glm::vec3(0);
			points[1] = glm::vec3(0);
			points[2] = glm::vec3(0);
			points[3] = glm::vec3(0);
		}

		drawLine(points[0], points[1], color);
		drawLine(points[2], points[3], color);
		drawLine(points[0], points[2], color);
		drawLine(points[1], points[3], color);
	}

	void GizmoRenderer::render(SceneCamera& camera) {
		glm::mat4 camMatrix = glm::inverse(camera.transform.getMatrix());
		glm::mat4 projectionMatrix = camera.camera.getMatrix();
		glm::mat4 invertZ = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, -1));

		// Lets invert the z axis for engine convenience
		glm::mat4 cameraProjectionMatrix = projectionMatrix * invertZ * camMatrix;

		for (std::array<glm::vec3, 3>&line : m_lines) {
			RenderUtils::m_lineShader->bind();
			RenderUtils::m_lineShader->uploadUniformMat4("u_viewMatrix", cameraProjectionMatrix);
			RenderUtils::m_lineShader->uploadUniformFloat3("u_color", line[2]);

			RenderUtils::m_lineShader->uploadUniformFloat3("u_posA", line[0]);
			RenderUtils::m_lineShader->uploadUniformFloat3("u_posB", line[1]);

			Render::submitLine(RenderUtils::m_lineVertexArray);
		}

	}

	void GizmoRenderer::refresh() {
		m_lines.clear();
	}
}
