#pragma once
#include "Deer/Core/Core.h"
#include "Deer/Core/Log.h"

#include "Deer/DataStore/Path.h"
#include "Deer/DataStore/DataAccess.h"
#include "Deer/DataStore/DataStore.h"

#include <string>

namespace Deer {
	template <typename T>
	class Asset {
	public:
		Asset() : m_assetID(0), m_assetLocation("null") { }
		Asset(uid id, const std::filesystem::path& assetLocation)
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

		inline uid getAssetID() const { return m_assetID; }
		inline Path& getAssetLocation() { return m_assetLocation; }

		Ref<T> value;
	private:
		uid m_assetID;
		Path m_assetLocation;
	};

	template <>
	class Asset<void> {
	public:
		Asset() : m_assetID(0), m_assetLocation("null") { }
		Asset(uid id, const std::filesystem::path& assetLocation)
			: m_assetID(id), m_assetLocation(assetLocation) { }

		inline uid getAssetID() const { return m_assetID; }
		inline Path& getAssetLocation() { return m_assetLocation; }

		Ref<void> value;
	private:
		uid m_assetID;
		Path m_assetLocation;
	};
}

