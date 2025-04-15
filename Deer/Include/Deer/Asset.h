#pragma once
#include <string>
#include <vector>

#include "Deer/DataStore.h"
#include "Deer/Log.h"
#include "Deer/Path.h"

namespace Deer {
	template <typename T>
	class Asset {
	   public:
		Asset() : m_assetID(0), m_assetLocation("null") {}
		Asset(uint32_t id, const std::filesystem::path& assetLocation)
		    : m_assetID(id), m_assetLocation(assetLocation) {
			try {
				uint32_t size;
				uint8_t* data = DataStore::readFile(assetLocation, &size);

				value = T::create(data, size);

				delete[] data;
			} catch (const std::string& error) {
				DEER_CORE_ERROR("Error loading asset {0}\n{1}",
				                assetLocation.generic_string().c_str(),
				                error.c_str());
			}
		}

		inline uint32_t getAssetID() const { return m_assetID; }
		inline Path& getAssetLocation() { return m_assetLocation; }

		Ref<T> value;

	   private:
		uint32_t m_assetID;
		Path m_assetLocation;
	};

	template <>
	class Asset<void> {
	   public:
		Asset() : m_assetID(0), m_assetLocation("null") {}
		Asset(uint32_t id, const std::filesystem::path& assetLocation)
		    : m_assetID(id), m_assetLocation(assetLocation) {}

		inline uint32_t getAssetID() const { return m_assetID; }
		inline Path& getAssetLocation() { return m_assetLocation; }

		Ref<void> value;

	   private:
		uint32_t m_assetID;
		Path m_assetLocation;
	};

	namespace AssetManager {
		extern std::vector<Asset<void>> assets;

		template <typename T>
		inline Asset<T>& getAsset(uint32_t assetID) {
			return *(Asset<T>*)&(assets[assetID]);
		}

		template <typename T>
		inline uint32_t loadAsset(const std::filesystem::path& assetLocation) {
			for (size_t id = 0; id < assets.size(); ++id) {
				if (assets[id].getAssetLocation() == assetLocation) return id;
			}

			uint32_t assetID = assets.size();

			Asset<T> asset(assetID, assetLocation);
			assets.push_back(*(Asset<void>*)&(asset));
			return assetID;
		}

		inline const std::filesystem::path getAssetLocation(uint32_t assetID) {
			return assets[assetID].getAssetLocation();
		}
	}  // namespace AssetManager
}  // namespace Deer
