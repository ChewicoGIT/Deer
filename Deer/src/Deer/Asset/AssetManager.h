#pragma once
#include "Deer/Core/Core.h"
#include "Deer/Asset/Asset.h"

#include <vector>
#include <string>

namespace Deer {
	class Texture;

	class AssetManager {
	public:
		AssetManager() { m_assets.push_back(Asset<void>()); }

		template<typename T>
		Asset<T>& getAsset(uid assetID) { return *(Asset<T>*)&(m_assets[assetID]); }

		const std::filesystem::path getAssetLocation (uid assetID) { return m_assets[assetID].getAssetLocation(); }

		template<typename T>
		uid loadAsset(const std::filesystem::path& assetLocation) {
			
			for (size_t id = 0; id < m_assets.size(); ++id) {
				if (m_assets[id].getAssetLocation() == assetLocation)
					return id;
			}

			uid assetID = m_assets.size();

			Asset<T> asset(assetID, assetLocation);
			m_assets.push_back(*(Asset<void>*)&(asset));
			return assetID;
		}

	private:
		std::vector<Asset<void>> m_assets;
	};
}


