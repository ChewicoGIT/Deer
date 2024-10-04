#include "ScriptEngine.h"
#include "Deer/Core/Log.h"

#include "ScriptEngineFunction.h"

#include "angelscript.h"
#include "scriptbuilder.h"
#include "scriptstdstring.h"

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

	void ScriptEngine::loadRoeModule(const std::filesystem::path& modulePath) {
		loadModuleFolder(modulePath, "Roe");

		m_roeModule = m_scriptEngine->GetModule("Roe");
		int classCount = m_roeModule->GetObjectTypeCount();

		DEER_CORE_INFO("Types : {0}", classCount);

		for (int i = 0; i < classCount; i++) {

			asITypeInfo* type = m_roeModule->GetObjectTypeByIndex(i);

			DEER_CORE_INFO("Inside engine there is : {0}.{1}",
				type->GetNamespace(), type->GetName());

			int props = type->GetPropertyCount();
			for (int x = 0; x < props; x++) {
				const char* subType = type->GetPropertyDeclaration(x, false);
				DEER_CORE_INFO("Deeper: {0}", subType);
			}
		}

	}

	void ScriptEngine::test() {
		asIScriptFunction* func = m_roeModule->GetFunctionByDecl("void main()");
		DEER_SCRIPT_ASSERT(func != 0, "The script must have the function 'void main()'. Please add it and try again.");

		// Create our context, prepare it, and then execute
		asIScriptContext* ctx = m_scriptEngine->CreateContext();
		ctx->Prepare(func);
		int r = ctx->Execute();
		if (r != asEXECUTION_FINISHED) {
			if (r == asEXECUTION_EXCEPTION) {
				DEER_SCRIPT_ERROR("An exception {0} occurred. Please correct the code and try again.", ctx->GetExceptionString());
			}
		}
	}

	void ScriptEngine::loadModuleFolder(const std::filesystem::path& modulePath, const char* moduleName) {
		CScriptBuilder builder;

		int r = builder.StartNewModule(m_scriptEngine, moduleName);
		DEER_SCRIPT_ASSERT(r >= 0, "Unrecoverable error while starting a new module. {0}", moduleName);

		r = builder.AddSectionFromFile(modulePath.generic_string().c_str());
		DEER_SCRIPT_ASSERT(r >= 0, "Please correct the errors in the script and try again. {0}", modulePath.generic_string().c_str());
		
		r = builder.BuildModule();
		DEER_SCRIPT_ASSERT(r >= 0, "Please correct the errors in the script and try again.");
	}
}