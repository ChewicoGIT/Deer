#include "Project.h"

#include "Deer/Asset/AssetManager.h"
#include "Deer/Scripting/ScriptEngine.h"

namespace Deer {
	AssetManager* Project::m_assetManager = new AssetManager();
	ScriptEngine* Project::m_scriptEngine = new ScriptEngine();
}