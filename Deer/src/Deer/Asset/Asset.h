#pragma once
#include "Deer/Core/Core.h"

#include <string>

namespace Deer {
	class Texture;
	class VertexArray;

	enum class AssetType {
		None = 0,
		Texture,
		Mesh
	};

	class Asset {
	public:
		Asset() : m_assetID(0), m_assetLocation("null") { }
		Asset(uid id, const std::string& assetLocation)
			: m_assetID(id), m_assetLocation(assetLocation) { }
		virtual ~Asset() = default;

		static AssetType getAssetTypeStatic() { return AssetType::None; }
		virtual AssetType getAssetType() { return AssetType::None; }

		inline uid getAssetID() const { return m_assetID; }
		inline std::string& getAssetLocation() { return m_assetLocation; }
	private:
		uid m_assetID;
		std::string m_assetLocation;
	};

	class TextureAsset : public Asset {
	public:
		TextureAsset(uid id, const std::string& assetLocation);

		static AssetType getAssetTypeStatic() { return AssetType::Texture; }
		AssetType getAssetType() override { return AssetType::Texture; }
		
		Ref<Texture> m_texture;
	};

	class MeshAsset : public Asset {
	public:
		MeshAsset(uid id, const std::string& assetLocation);

		static AssetType getAssetTypeStatic() { return AssetType::Mesh; }
		AssetType getAssetType() override { return AssetType::Mesh; }

		Ref<VertexArray> m_mesh;
	};
}

