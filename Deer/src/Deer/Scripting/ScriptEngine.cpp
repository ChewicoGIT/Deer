#include "ScriptEngine.h"
#include "Deer/Core/Log.h"

#include "ScriptEngineFunction.h"

#include "angelscript.h"
#include "scriptbuilder.h"
#include "scriptstdstring.h"
#include "Deer/Scripting/RoeScript.h"

#include <filesystem>

namespace fs = std::filesystem;

namespace Deer {
	void ScriptEngine::initScriptEngine() {
		m_scriptEngine = asCreateScriptEngine();

		// Set the message callback to receive information on errors in human readable form.
		int r = m_scriptEngine->SetMessageCallback(asFUNCTION(Deer::messageCallback), 0, asCALL_CDECL);
		DEER_SCRIPT_ASSERT(r >= 0, "Error in seting up angel script");

		RegisterStdString(m_scriptEngine);

		r = m_scriptEngine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(Deer::print), asCALL_CDECL);
		DEER_SCRIPT_ASSERT(r >= 0, "Error in seting up void print(const string &in)");
	}

	void ScriptEngine::shutdownScriptEngine() {
		m_scriptEngine->ShutDownAndRelease();
	}

	void ScriptEngine::loadRoeModule(const std::filesystem::path& modulePath) {
		loadModuleFolder(modulePath, "Roe");
		m_roeModule = m_scriptEngine->GetModule("Roe");
		m_deerScript = m_roeModule->GetTypeInfoByName("DeerScript");

		int classCount = m_roeModule->GetObjectTypeCount();

		for (int i = 0; i < classCount; i++) {
			asITypeInfo* type = m_roeModule->GetObjectTypeByIndex(i);

			asITypeInfo* parent = type->GetBaseType();

			if (parent == m_deerScript)
				m_deerScripts.push_back({ type });
		}
	}

	void ScriptEngine::loadDeerModule(const std::filesystem::path& modulePath) {
		loadModuleFolder(modulePath, "Deer");

		m_deerModule = m_scriptEngine->GetModule("Deer");
		m_deerScript = m_deerModule->GetTypeInfoByName("DeerScript");
	}

	uid ScriptEngine::createScriptInstance(uid scriptID) {
		return uid();
	}


	void ScriptEngine::loadModuleFolder(const std::filesystem::path& modulePath, const char* moduleName) {
		CScriptBuilder builder;

		int r = builder.StartNewModule(m_scriptEngine, moduleName);
		DEER_SCRIPT_ASSERT(r >= 0, "Unrecoverable error while starting a new module. {0}", moduleName);

		try {
			for (const auto& entry : fs::recursive_directory_iterator(modulePath)) {
				if (fs::is_regular_file(entry) && entry.path().extension() == ".as") {

					r = builder.AddSectionFromFile(entry.path().generic_string().c_str());
					DEER_SCRIPT_ASSERT(r >= 0, "Please correct the errors in the script and try again. {0}", entry.path().generic_string().c_str());

					DEER_SCRIPT_TRACE("Loading script : {0}", entry.path().filename().string().c_str());
				}
			}
		} catch (const fs::filesystem_error& e) {
			DEER_CORE_ERROR("Error while loading scripts, error: {0}", e.what());
		}

		r = builder.BuildModule();
		DEER_SCRIPT_ASSERT(r >= 0, "Please correct the errors in the script and try again.");
	}
}