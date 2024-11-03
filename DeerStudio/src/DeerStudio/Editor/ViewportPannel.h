#pragma once
#include "DeerStudio/Editor/EditorPannel.h"
#include "DeerStudio/Editor/ActiveEntity.h"

#include "Deer/Scene/Enviroment.h"
#include "Deer/Scene/Entity.h"

#include "Deer/Core/Events/Event.h"
#include "Deer/Core/Events/keyEvent.h"
#include "Deer/Core/Events/MouseEvent.h"

#include <string>

namespace Deer {
	enum class TransformMode {
		Translate = 0,
		Rotate = 1,
		Scale = 2
	};

	class ViewportPannel : public EditorPannel {
	public:
		ViewportPannel(Ref<Environment>& enviroment, const std::string& windowName, Ref<ActiveEntity>& activeEntity);
		
		void onImGui() override;
		void onRender(Timestep timestep) override;
		void onEventCallback(Event& e) override;
		bool onClickEvent(MouseButtonPressedEvent mouseEvent);
		bool onKeyEvent(KeyPressedEvent keyEvent);
	private:
		// bool expresses if it is being used
		bool drawGizmos(int wPosX, int wPosY, int wSizeX, int wSizeY);
	private:
		Ref<Environment> m_environment;
		Ref<FrameBuffer> m_frameBuffer;
		Ref<ActiveEntity> m_activeEntity;

		std::string m_windowName;

		glm::vec2 m_lastWindowSize; 
		glm::vec2 m_lastMousePos;

		bool m_lastMousePressedButton1;
		bool m_isActive;
		bool m_handleClick;

		float m_sensitivity = 0.005f;

		TransformMode m_transformMode = TransformMode::Translate;
		VirtualCamera m_virtualCamera;
	};
}

