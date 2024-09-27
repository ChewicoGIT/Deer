#pragma once
#include "DeerStudio/Editor/EditorPannel.h"
#include "Deer/Core/Core.h"

#include <filesystem>

namespace Deer {
	class Texture2D;
	class SceneSerializer;

	class AssetManagerPannel : public EditorPannel{
	public:
		AssetManagerPannel(); // Ref<SceneSerializer> sceneSerializer
		void onImGui() override;
	private:
		void drawFolder(std::filesystem::path);
		void drawFile(std::filesystem::path);
		void updateContextMenu();
	private:
		std::filesystem::path m_currentPath; // Assets/path...
		Ref<Texture2D> m_folderIcon;
		Ref<Texture2D> m_fileIcon;
		Ref<SceneSerializer> m_sceneSerializer;

		int m_iconMinSize = 130;
	};
}

