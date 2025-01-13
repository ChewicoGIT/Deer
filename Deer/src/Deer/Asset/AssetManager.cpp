#include "AssetManager.h"

namespace Deer {
	std::vector<Asset<void>> AssetManager::assets;
	DataAccess* AssetManager::dataAccess;

	void AssetManager::setupDataAccess(DataAccess* _dataAccess) {
		if (dataAccess)
			delete dataAccess;

		dataAccess = _dataAccess;
	}
}

