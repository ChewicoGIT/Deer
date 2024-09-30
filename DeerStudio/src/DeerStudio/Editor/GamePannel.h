#pragma once
#include "Deer/Core/Core.h"
#include "DeerStudio/Editor/EditorPannel.h"
#include "glm/glm.hpp"

namespace Deer {
	class Scene;
	class FrameBuffer;

	class GamePannel : public EditorPannel {
	public:
		GamePannel(Ref<Scene> scene);
		void onImGui() override;

	private:
		Ref<Scene> m_scene;
		Ref<FrameBuffer> m_frameBuffer;

		glm::vec2 m_lastWindowSize;
	};
}

