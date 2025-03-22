#pragma once
#include "Deer/Asset.h"

#include <vector>
#include <string>

namespace Deer {
	namespace AssetManager {
		extern std::vector<Asset<void>> assets;

		template<typename T>
		inline Asset<T>& getAsset(uint32_t assetID) { return *(Asset<T>*) & (assets[assetID]); }

		template<typename T>
		inline uint32_t loadAsset(const std::filesystem::path& assetLocation) {
			for (size_t id = 0; id < assets.size(); ++id) {
				if (assets[id].getAssetLocation() == assetLocation)
					return id;
			}

			uint32_t assetID = assets.size();

			Asset<T> asset(assetID, assetLocation);
			assets.push_back(*(Asset<void>*) & (asset));
			return assetID;
		}

		inline const std::filesystem::path getAssetLocation(uint32_t assetID) { return assets[assetID].getAssetLocation(); }
	}
}


