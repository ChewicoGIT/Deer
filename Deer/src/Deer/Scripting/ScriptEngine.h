#pragma once
#include <filesystem>
#include <string>

class asIScriptEngine;
class asIScriptModule;

namespace Deer {
	class ScriptEngine {
	public:
		ScriptEngine() = default;

		void initScriptEngine();
		void shutdownScriptEngine();

		void loadRoeModule(const std::filesystem::path& modulePath);

		void test();

	private:
		asIScriptEngine* m_scriptEngine;
		asIScriptModule* m_roeModule;

		void loadModuleFolder(const std::filesystem::path& modulePath, const char* moduleName);
	};
}

