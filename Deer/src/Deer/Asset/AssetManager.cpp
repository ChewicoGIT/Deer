#include "AssetManager.h"
#include "Deer/Core/Log.h"

#include "Deer/Render/Texture.h"

#include <memory>

namespace Deer {
	Ref<Asset>& AssetManager::getAsset(uid assetID) {
		return m_assets[assetID];
	}

	uid AssetManager::loadAsset(AssetType assetType, const std::string& assetLocation) {
		
		Ref<Asset> asset;
		uid assetID = m_assets.size();

		switch (assetType)
		{
		case Deer::AssetType::None:
			break;
		case Deer::AssetType::Texture:
			break;
		case Deer::AssetType::Mesh:
			asset = Ref<Asset>(new MeshAsset(assetID, assetLocation));
			break;
		default:
			DEER_CORE_ERROR("Asset type NONE is not supported");
			return 0;
		}

		m_assets.push_back(asset);
		return assetID;
	}
}