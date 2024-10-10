#pragma once
#include "Deer/Core/Core.h"

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include "Deer/Scripting/ComponentScript.h"

class asIScriptEngine;
class asIScriptModule;
class asIScriptContext;
class asIScriptFunction;
class asITypeInfo;

namespace Deer {
	class Scene;
	class Entity;

	using ComponentScriptMap = std::unordered_map<std::string, ComponentScript>;

	class ScriptEngine {
	public:
		ScriptEngine() = default;

		void initScriptEngine();
		void shutdownScriptEngine();

		void beginExecutionContext();
		void endExecutionContext();
		inline asIScriptContext* getExecutionContext() { return m_context; }

		void loadScripts(const std::filesystem::path& scriptPath);

		inline ComponentScriptMap& getComponentScripts() { return m_componentScripts; }
		inline ComponentScript& getComponentScript(const std::string& scriptID) { return m_componentScripts[scriptID]; }
		
		Ref<ComponentScriptInstance> createComponentScriptInstance(const std::string& scriptID, Entity& scriptEntity);
	private:
		asIScriptEngine* m_scriptEngine;
		asIScriptModule* m_scriptModule;

		asIScriptContext* m_context;
		ComponentScriptMap m_componentScripts;

		void loadModuleFolder(const std::filesystem::path& modulePath, const char* moduleName);
	};
}

