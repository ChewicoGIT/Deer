#pragma once

#include "Deer/Log.h"

#include "Deer/Path.h"
#include "Deer/DataStore/DataAccess.h"
#include "Deer/DataStore.h"

#include <string>

namespace Deer {
	template <typename T>
	class Asset {
	public:
		Asset() : m_assetID(0), m_assetLocation("null") { }
		Asset(uint32_t id, const std::filesystem::path& assetLocation)
			: m_assetID(id), m_assetLocation(assetLocation) {
			try {
				uint32_t size;
				uint8_t* data = DataStore::dataAccess->loadData(assetLocation, &size);

				value = T::create(data, size);

				DataStore::dataAccess->freeData(assetLocation, data);
			}
			catch (const std::string& error){
				DEER_CORE_ERROR("Error loading asset {0}\n{1}", assetLocation.generic_string().c_str(), error.c_str());
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
		Asset() : m_assetID(0), m_assetLocation("null") { }
		Asset(uint32_t id, const std::filesystem::path& assetLocation)
			: m_assetID(id), m_assetLocation(assetLocation) { }

		inline uint32_t getAssetID() const { return m_assetID; }
		inline Path& getAssetLocation() { return m_assetLocation; }

		Ref<void> value;
	private:
		uint32_t m_assetID;
		Path m_assetLocation;
	};
}

