#pragma once
#include "Deer/Core/Core.h"
#include "Deer/Asset/Asset.h"

#include <vector>
#include <string>

namespace Deer {
	class Texture;

	class AssetManager {
	public:
		AssetManager() { m_assets.push_back(Ref<Asset>(new Asset())); }

		Ref<Asset>& getAsset(uid assetID);
		uid loadAsset(AssetType assetType, const std::string& assetLocation);

	private:
		std::vector<Ref<Asset>> m_assets;
	};
}


