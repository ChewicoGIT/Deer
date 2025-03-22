#pragma once
#include "Deer/Memory.h"
#include "Deer/ComponentScript.h"

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

class asIScriptEngine;
class asIScriptModule;
class asIScriptContext;
class asIScriptFunction;
class asITypeInfo;

namespace Deer {
	class Scene;
	class Entity;

	using ComponentScriptMap = std::unordered_map<std::string, ComponentScript>;

	namespace ScriptEngine {
		extern asIScriptContext* m_context;
		extern bool m_isCompilationValid;
		extern ComponentScriptMap m_componentScripts;
		extern Scene* m_scene;

		void compileScriptEngine(const std::filesystem::path& scriptPath);
		void shutdownScriptEngine();

		void beginExecutionContext(Scene* executingScene);
		void endExecutionContext();

		inline asIScriptContext* getExecutionContext() { return m_context; }
		inline bool isCompilationValid() { return m_isCompilationValid; }

		inline ComponentScriptMap& getComponentScripts() { return m_componentScripts; }
		inline ComponentScript& getComponentScript(const std::string& scriptID) { return m_componentScripts[scriptID]; }
		
		Ref<ComponentScriptInstance> createComponentScriptInstance(const std::string& scriptID, Entity& scriptEntity);
	}
}

