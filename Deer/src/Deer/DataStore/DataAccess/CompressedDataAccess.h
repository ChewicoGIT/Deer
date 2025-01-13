#pragma once
#include "Deer/Core/Core.h"
#include "Deer/DataStore/DataAccess.h"
#include "Deer/DataStore/Path.h"
#include "Deer/DataStore/DataStructure.h"

#include <unordered_map>

namespace Deer {
	class CompressedDataAccess : public DataAccess{
	public:
		CompressedDataAccess(uint8_t* _data) : m_data(_data) {}
		~CompressedDataAccess() override;

		uint8_t* loadData(const Path& path, uint32_t* size) override;
		virtual void freeData(const Path& path, uint8_t* data) override { }

	private:
		uint8_t* m_data;
		std::unordered_map<Path, DataStructure> m_dataStructure;
	};
}

