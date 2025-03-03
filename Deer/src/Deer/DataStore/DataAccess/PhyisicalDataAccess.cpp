#include "PhyisicalDataAccess.h"
#include "Deer/DataStore/DataStore.h"

namespace Deer {
	uint8_t* Deer::PhyisicalDataAccess::loadData(const Path& path, uint32_t* size) {
		return DataStore::readFile(DataStore::rootPath / path, size);
	}

	void Deer::PhyisicalDataAccess::freeData(const Path& path, uint8_t* data) {
		delete[] data;
	}
}
