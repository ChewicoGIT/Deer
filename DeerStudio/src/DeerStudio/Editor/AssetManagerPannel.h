#pragma once
#include "DeerStudio/Editor/EditorPannel.h"
#include "Deer/Core/Core.h"

#include <filesystem>

namespace Deer {
	class Texture2D;
	class DeerStudioLayer;
	class SceneSerializer;
	class ActiveEntity;

	class AssetManagerPannel : public EditorPannel{
	public:
		AssetManagerPannel(Ref<SceneSerializer>, Ref<ActiveEntity>);
		void onImGui() override;
	private:
		void drawFolder(const std::filesystem::path&);
		void drawFile(const std::filesystem::path&);
		void updateContextMenu();
	private:
		std::filesystem::path m_currentScenePath;
		Ref<SceneSerializer> m_sceneSerializer;
		Ref<ActiveEntity> m_activeEntity;

		std::filesystem::path m_currentPath; // Assets/path...
		Ref<Texture2D> m_folderIcon;
		Ref<Texture2D> m_fileIcon;
		Ref<Texture2D> m_scneIcon;
		Ref<Texture2D> m_objectIcon;

		int m_iconMinSize = 110;
		bool m_clickHandled = false;
	};
}

