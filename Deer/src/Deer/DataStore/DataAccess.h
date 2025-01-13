#pragma once
#include "Deer/Core/Core.h"
#include "Deer/DataStore/Path.h"

namespace Deer {
	class DataAccess {
	public:
		virtual ~DataAccess() = 0;

		virtual uint8_t* loadData(const Path& path, uint32_t* size) = 0;
		virtual void freeData(const Path& path, uint8_t* data) = 0;
	};
}

