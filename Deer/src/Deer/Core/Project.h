#pragma once
#include "Deer/Asset/AssetManager.h"
#include "Deer/Scripting/ScriptEngine.h"

namespace Deer {
	class Project {
	public:
		static AssetManager m_assetManager;
		static ScriptEngine m_scriptEngine;
	};
}

