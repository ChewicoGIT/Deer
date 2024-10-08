#pragma once
#include "Deer/Core/Core.h"

#include <filesystem>
#include <string>
#include <vector>

#include "Deer/Scripting/DeerScript.h"
#include "Deer/Scripting/ScriptInstance.h"

class asIScriptEngine;
class asIScriptModule;
class asIScriptContext;
class asIScriptFunction;
class asITypeInfo;

namespace Deer {
	class Scene;

	class ScriptEngine {
	public:
		ScriptEngine() = default;

		void initScriptEngine();
		void shutdownScriptEngine();

		void beginExecutionContext();
		void endExecutionContext();

		void loadScripts(const std::filesystem::path& modulePath);

		uid createScriptInstance(uid scriptID);
		void updateRoeInstance(uid scriptInstance);

		ScriptInstance& getScriptInstance(uid instanceID) { return m_deerObjects[instanceID]; }
		inline std::vector<DeerScript>& getScript() { return m_deerScripts; }
	private:
		asIScriptEngine* m_scriptEngine;
		asIScriptModule* m_roeModule;

		asIScriptContext* m_context;
		asITypeInfo* m_deerScript;

		std::vector<DeerScript> m_deerScripts;
		std::vector<ScriptInstance> m_deerObjects;

		void loadModuleFolder(const std::filesystem::path& modulePath, const char* moduleName);
	};
}

