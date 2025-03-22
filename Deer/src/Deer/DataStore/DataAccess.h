#pragma once

#include "Deer/Path.h"

namespace Deer {
	class DataAccess {
	public:
		virtual ~DataAccess() = default;

		virtual uint8_t* loadData(const Path& path, uint32_t* size) = 0;
		virtual void freeData(const Path& path, uint8_t* data) = 0;

		virtual bool isDataBin() = 0;
	};

}

