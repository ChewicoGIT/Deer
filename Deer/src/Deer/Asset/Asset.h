#pragma once
#include "Deer/Core/Core.h"

#include <string>

namespace Deer {
	template <typename T>
	class Asset {
	public:
		Asset() : m_assetID(0), m_assetLocation("null") { }
		Asset(uid id, const std::string& assetLocation)
			: m_assetID(id), m_assetLocation(assetLocation) {
			value = T::create(assetLocation);
		}

		inline uid getAssetID() const { return m_assetID; }
		inline std::string& getAssetLocation() { return m_assetLocation; }

		Ref<T> value;
	private:
		uid m_assetID;
		std::string m_assetLocation;
	};

	template <>
	class Asset<void> {
	public:
		Asset() : m_assetID(0), m_assetLocation("null") { }
		Asset(uid id, const std::string& assetLocation)
			: m_assetID(id), m_assetLocation(assetLocation) { }

		inline uid getAssetID() const { return m_assetID; }
		inline std::string& getAssetLocation() { return m_assetLocation; }

		Ref<void> value;
	private:
		uid m_assetID;
		std::string m_assetLocation;
	};
}

