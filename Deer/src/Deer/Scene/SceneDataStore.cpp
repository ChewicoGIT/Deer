#include "SceneDataStore.h"
#include "cereal/archives/json.hpp"
#include "cereal/archives/portable_binary.hpp"

#include "Deer/Scene/Serialization/Serialization.h"

#include <vector>
#include <string>
#include <sstream>

namespace Deer {
	Scene loadSceneJson(uint8_t* data, uint32_t size);
	Scene loadSceneBin(uint8_t* data, uint32_t size);

	Scene SceneDataStore::loadScene(const Path& name) {
		
		uint32_t size;
		uint8_t* data = DataStore::dataAccess->loadData(name, &size);

		Scene scene_data;
		if (DataStore::dataAccess->isDataBin())
			scene_data = loadSceneBin(data, size);
		else
			scene_data = loadSceneJson(data, size);

		DataStore::dataAccess->freeData(name, data);
		return scene_data;
	}

	Scene loadSceneJson(uint8_t* data, uint32_t size) {
		std::string strData((char*)data, size);
		std::istringstream stream(strData);

		Scene scene;
		{
			cereal::JSONInputArchive archive(stream);
			archive(cereal::make_nvp("scene", scene));
		}

		return scene;
	}

	Scene loadSceneBin(uint8_t* data, uint32_t size) {
		std::string strData((char*)data, size);
		std::istringstream stream(strData);

		Scene scene;
		{
			cereal::PortableBinaryInputArchive archive(stream);
			archive(cereal::make_nvp("scene", scene));
		}

		return scene;
	}

	void SceneDataStore::exportSceneJson(Scene& scene, const Path& name) {
		is_server_serialization = false;

		std::stringstream output;
		{
			cereal::JSONOutputArchive archive(output);
			archive(cereal::make_nvp("scene", scene));
		}

		Path savePath = Path(DEER_SCENE_PATH) / (name.generic_string() + ".dscn");
		std::string_view view = output.view();

		DataStore::saveFile(savePath, (uint8_t*)view.data(), view.size());
	}

	void exportSceneBin(Scene& scene, const Path& name) {
		is_server_serialization = false;

		std::stringstream output;
		{
			cereal::PortableBinaryOutputArchive archive(output);
			archive(cereal::make_nvp("scene", scene));
		}

		Path savePath = Path(DEER_BIN_PATH) / Path(DEER_SCENE_PATH) / (name.generic_string() + ".dbscn");
		std::string_view view = output.view();

		DataStore::saveFile(savePath, (uint8_t*)view.data(), view.size());
	}

	void SceneDataStore::exportScenesBin() {
		std::vector<Path> scenes = DataStore::getFiles(DEER_SCENE_PATH, ".dscn");
		
		for (Path& scene_path : scenes) {
			uint32_t size;
			uint8_t* data = DataStore::readFile(scene_path, &size);

			Scene scene_data = loadSceneJson(data, size);
			delete[] data;

			Path name = scene_path.lexically_relative(DEER_SCENE_PATH);
			name = name.root_path() / name.stem();

			exportSceneBin(scene_data, name);
		}
	}

	void SceneDataStore::exportRuntimeScene(Scene& scene) {
		std::stringstream output;
		{
			cereal::PortableBinaryOutputArchive archive(output);
			archive(cereal::make_nvp("scene", scene));
		}

		Path savePath = Path(DEER_TEMP_PATH) / "scene_runtime.dbscn";
		std::string_view view = output.view();

		DataStore::saveFile(savePath, (uint8_t*)view.data(), view.size());
	}

	Scene SceneDataStore::importRuntimeScene() {
		Path loadPath = Path(DEER_TEMP_PATH) / "scene_runtime.dbscn";

		uint32_t size;
		uint8_t* data = DataStore::readFile(loadPath, &size);

		Scene scene_data = loadSceneBin(data, size);
		delete[] data;

		return scene_data;
	}
}
