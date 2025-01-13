#pragma once
#include "Deer/Core/Core.h"
#include "Deer/DataStore/DataAccess.h"

namespace Deer {
	namespace AssetManager {
		extern DataAccess* dataAccess;
		void setupDataAccess(DataAccess* _dataAccess);
	}
}


