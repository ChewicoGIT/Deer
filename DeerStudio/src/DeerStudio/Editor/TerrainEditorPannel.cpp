#include "TerrainEditorPannel.h"
#include "Deer/Core/Project.h"
#include "Deer/Voxels/VoxelWorld.h"
#include "Deer/Scene/Scene.h"
#include "imgui.h"

namespace Deer {
	void Deer::TerrainEditorPannel::onImGui() {
		ImGui::Begin("Terrain Editor");


		if (ImGui::Button("Create Terrain")) {

            for (int x = 0; x < 32; x++) {
                for (int y = 0; y < 32; y++) {
                    for (int z = 0; z < 32; z++) {
                        int rx = x - 16;
                        int ry = y - 16;
                        int rz = z - 16;
                        int distance = rx * rx + ry * ry + rz * rz;

                        if (distance < 14 * 14)
                            Project::m_scene->getVoxelWorld()->modVoxel(x, y, z).id = 1;
                    }
                }
            }

            Project::m_scene->getVoxelWorld()->bakeChunk(0, 0, 0);
		}

		ImGui::End();
	}
}
