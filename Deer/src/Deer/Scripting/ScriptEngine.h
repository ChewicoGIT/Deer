#pragma once
#include "Deer/Core/Core.h"

#include <filesystem>
#include <string>
#include <vector>

#include "Deer/Scripting/RoeScript.h"
#include "Deer/Scripting/RoeInstance.h"

class asIScriptEngine;
class asIScriptModule;
class asITypeInfo;

namespace Deer {

	class ScriptEngine {
	public:
		ScriptEngine() = default;

		void initScriptEngine();
		void shutdownScriptEngine();

		void loadRoeModule(const std::filesystem::path& modulePath);
		void loadDeerModule(const std::filesystem::path& modulePath);

		uid createScriptInstance(uid scriptID);
		inline std::vector<RoeScript>& getRoeScripts() { return m_deerScripts; }

	private:
		asIScriptEngine* m_scriptEngine;

		asIScriptModule* m_roeModule;
		asIScriptModule* m_deerModule;

		asITypeInfo* m_deerScript;

		std::vector<RoeScript> m_deerScripts;
		std::vector<RoeInstance> m_deerObjects;

		void loadModuleFolder(const std::filesystem::path& modulePath, const char* moduleName);
	};
}

