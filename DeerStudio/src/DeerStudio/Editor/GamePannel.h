#pragma once
#include "Deer/Core/Core.h"
#include "DeerStudio/Editor/EditorPannel.h"
#include "DeerStudio/Editor/ActiveEntity.h"
#include "glm/glm.hpp"

namespace Deer {
	class Scene;
	class FrameBuffer;

	class GamePannel : public EditorPannel {
	public:
		GamePannel(Ref<ActiveEntity>& activeEntity);
		void onImGui() override;

	private:
		Ref<FrameBuffer> m_frameBuffer;
		Ref<ActiveEntity> m_activeEntity;

		glm::vec2 m_lastWindowSize;
	};
}

