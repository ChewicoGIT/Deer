#pragma once
#include "glm/glm.hpp"

#include <vector>
#include <array>

namespace Deer {
	struct SceneCamera;
	struct GizmoFace {
		glm::vec3 positions[4];
		uint16_t textureID;
		uint8_t face;
	};

	class GizmoRenderer {
	public:
		void drawLine(glm::vec3 a, glm::vec3 b, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
		void drawVoxelLine(int x, int y, int z, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
		void drawVoxelLineFace(int x, int y, int z, uint8_t face, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
		void drawVoxelFace(int x, int y, int z, uint16_t voxelID, uint8_t face);
		void drawVoxelFaceInternal(int x, int y, int z, uint16_t voxelID, uint8_t face);

		void render(const SceneCamera& camera);
		void refresh();
	private:
		std::vector<std::array<glm::vec3, 3>> m_lines;
		std::vector<GizmoFace> m_internalFaces;
		std::vector<GizmoFace> m_faces;
	};
}

