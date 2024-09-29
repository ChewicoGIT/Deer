#pragma once
#include "Deer/Core/Core.h"
#include "DeerStudio/Editor/EditorPannel.h"
#include "glm/glm.hpp"

namespace Deer {
	class Environment;
	class FrameBuffer;

	class GamePannel : public EditorPannel {
	public:
		GamePannel(Ref<Environment>);
		void onImGui() override;

	private:
		Ref<Environment> m_environment;
		Ref<FrameBuffer> m_frameBuffer;

		glm::vec2 m_lastWindowSize;

	};
}

