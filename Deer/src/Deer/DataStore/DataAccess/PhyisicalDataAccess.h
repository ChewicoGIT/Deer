#pragma once
#include "Deer/DataStore/DataAccess.h"
#include "Deer/DataStore/DataStore.h"

namespace Deer {
	class PhyisicalDataAccess : public DataAccess{
	public:
		uint8_t* loadData(const Path& path, uint32_t* size) override;
		void freeData(const Path& path, uint8_t* data) override;

		virtual bool isDataBin() override { return false; }
	};
}

