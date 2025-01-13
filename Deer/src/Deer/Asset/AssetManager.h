#pragma once
#include "Deer/Core/Core.h"
#include "Deer/Asset/Asset.h"
#include "Deer/DataStore/DataAccess.h"

#include <vector>
#include <string>

namespace Deer {
	namespace AssetManager {
		extern std::vector<Asset<void>> assets;
		extern DataAccess* dataAccess;

		void setupDataAccess(DataAccess* _dataAccess);

		template<typename T>
		inline Asset<T>& getAsset(uid assetID) { return *(Asset<T>*) & (assets[assetID]); }

		template<typename T>
		inline uid loadAsset(const std::filesystem::path& assetLocation) {
			for (size_t id = 0; id < assets.size(); ++id) {
				if (assets[id].getAssetLocation() == assetLocation)
					return id;
			}

			uid assetID = assets.size();

			Asset<T> asset(assetID, assetLocation);
			assets.push_back(*(Asset<void>*) & (asset));
			return assetID;
		}

		inline const std::filesystem::path getAssetLocation(uid assetID) { return assets[assetID].getAssetLocation(); }
	}
}


