#include "VoxelPannel.h"
#include "imgui.h"

#include "Deer/Voxels/VoxelSerializer.h"

namespace Deer {
	void VoxelPannel::onImGui() {
        ImGui::Begin("Voxel");

		if (ImGui::Button("Create voxel list template")) {

			VoxelList vl;
			vl.voxelInfo.push_back(VoxelInfo(0));
			vl.voxelInfo.push_back(VoxelInfo(1));

			VoxelSerializer vs;
			vs.saveVoxels("voxels", vl);
		}

		ImGui::End();
	}
}