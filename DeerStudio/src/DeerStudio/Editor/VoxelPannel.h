#pragma once
#include "EditorPannel.h"

namespace Deer {
	class VoxelPannel : public EditorPannel {
	public:
		void onImGui() override;
	private:
		int m_selectedVoxel = 0;
	};
}
