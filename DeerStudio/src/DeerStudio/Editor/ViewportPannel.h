#pragma once
#include "DeerStudio/Editor/EditorPannel.h"
#include "Deer/Scene/Enviroment.h"
#include "Deer/Scene/Entity.h"

#include <string>

namespace Deer {
	enum class TransformMode {
		Translate = 0,
		Rotate = 1,
		Scale = 2
	};

	class ViewportPannel : public EditorPannel {
	public:
		ViewportPannel(Ref<Environment>& enviroment, const std::string& windowName = "viewport");
		void onImGui() override;
		void onUpdate(Timestep timestep) override;
		
		// Temp
		Entity m_entity;
	private:
		void drawGizmos(int wPosX, int wPosY, int wSizeX, int wSizeY);
	private:
		Ref<Environment> m_environment;
		Ref<FrameBuffer> m_frameBuffer;

		std::string m_windowName;
		glm::vec2 m_lastWindowSize; 
		glm::vec2 m_lastMousePos;
		bool m_lastMousePressedButton1;
		bool m_isActive;
		float m_sensitivity = 0.005f;

		TransformMode m_transformMode = TransformMode::Translate;

		VirtualCamera m_virtualCamera;
	};
}

