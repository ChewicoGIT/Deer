#pragma once
#include "Deer/Core/Layer.h"
#include "glm/glm.hpp"

#include "Deer/Core/Input.h"
#include "Deer/Core/Timestep.h"
#include "Deer/Core/Transform.h"
#include "Deer/Core/Core.h"

#include "Deer/Render/Buffer.h"
#include "Deer/Render/VertexArray.h"
#include "Deer/Render/Shader.h"
#include "Deer/Render/Camera.h"
#include "Deer/Render/Texture.h"
#include "Deer/Render/Framebuffer.h"

#include "Deer/Scene/Enviroment.h"
#include "Deer/Scene/Components.h"
#include "Deer/Scene/Entity.h"
#include "Deer/Scene/Scene.h"
#include "Deer/Scene/SceneSerializer.h"

#include "DeerStudio/Editor/EnviromentTreePannel.h"
#include "DeerStudio/Editor/PropertiesPannel.h"
#include "DeerStudio/Editor/ViewportPannel.h"
#include "DeerStudio/Editor/AssetManagerPannel.h"

#include <filesystem>
#include <vector>

namespace Deer {
	class DeerStudioLayer : public Layer {
		void onAttach() override;
		void onUpdate(Timestep delta) override;

		void loadScene();
		void unloadScene();

		void onEvent(Event& e) override;

		void onImGUI() override;
		void drawMenuBar();
	private:
		Ref<Texture2D> m_texture;

	private:
		uid m_meshID = 0;
		uid m_shaderID = 0;

		Ref<Scene> m_scene;
		Ref<ActiveEntity> m_activeEntity;
		Ref<SceneSerializer> m_sceneSerializer;
		std::vector<Ref<EditorPannel>> pannels;

	};
}

