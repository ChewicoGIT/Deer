#include "ScriptEngine.h"
#include "Deer/Core/Log.h"

#include "angelscript.h"
#include "scriptbuilder.h"
#include "scriptstdstring.h"
#include "scriptmath.h"

#include "ScriptEngineFunctions.h"

#include "Deer/Scripting/ComponentScript.h"
#include "Deer/Scene/Entity.h"

#include <filesystem>

namespace fs = std::filesystem;

namespace Deer {
	void ScriptEngine::initScriptEngine() {
		m_scriptEngine = asCreateScriptEngine();

		RegisterStdString(m_scriptEngine);
		RegisterScriptMath(m_scriptEngine);

		// Regist data types
		registerEntity(m_scriptEngine);
		registerVec3(m_scriptEngine);

		// Regist functions
		registerDeerFunctions(m_scriptEngine);
		registerInputFunctions(m_scriptEngine);
		registerEntityTransformFunctions(m_scriptEngine);
	}

	void ScriptEngine::shutdownScriptEngine() {
		m_componentScripts.clear();
		m_scriptEngine->ShutDownAndRelease();
	}

	void ScriptEngine::beginExecutionContext() {
		m_context = m_scriptEngine->CreateContext();
	}

	void ScriptEngine::endExecutionContext() {
		m_context->Release();
	}

	void ScriptEngine::loadScripts(const std::filesystem::path& modulePath) {
		loadModuleFolder(modulePath, "Deer");
		m_scriptModule = m_scriptEngine->GetModule("Deer");
		asITypeInfo* m_deerScript = m_scriptModule->GetTypeInfoByName("ComponentScript");

		int classCount = m_scriptModule->GetObjectTypeCount();
		for (int i = 0; i < classCount; i++) {
			asITypeInfo* type = m_scriptModule->GetObjectTypeByIndex(i);
			asITypeInfo* parent = type->GetBaseType();

			std::string scriptID = type->GetName();

			if (parent == m_deerScript) {
				ComponentScript componentScript(type);
				m_componentScripts.insert({ scriptID, componentScript });
			}
		}
	}

	Ref<ComponentScriptInstance> ScriptEngine::createComponentScriptInstance(const std::string& scriptID, Entity& scriptEntity) {
		ComponentScript& script = getComponentScript(scriptID);
		asITypeInfo* type = script.m_typeInfo;

		ComponentScriptInstance* instance = new ComponentScriptInstance();

		std::string factoryString(script.getName());
		factoryString = factoryString + " @" + script.getName() + "()";

		asIScriptFunction* function = type->GetFactoryByDecl(factoryString.c_str());
		if (!function) {
			DEER_SCRIPT_ERROR("Function constructor not found for class {0}", script.getName());
			return nullptr;
		}

		int r = m_context->Prepare(function);
		if (r < 0) {
			DEER_SCRIPT_ERROR("Failed to prepare constructor context for class {0}", script.getName());
			return nullptr;
		}

		r = m_context->Execute();
		if (r < 0) {
			DEER_SCRIPT_ERROR("Failed to execute constructor for class {0}", script.getName());
			return nullptr;
		}

		asIScriptObject* obj = *(asIScriptObject**)m_context->GetAddressOfReturnValue();
		obj->AddRef();

		int entityPosition = script.getAttribute("entity").internalID;
		unsigned int* entityValue = (unsigned int*)obj->GetAddressOfProperty(entityPosition);

		*entityValue = scriptEntity.getUID();
	
		asIScriptFunction* updateFunction = type->GetMethodByDecl("void update()");
		asIScriptFunction* startFunction = type->GetMethodByDecl("void start()");
		instance->m_updateFunction = updateFunction;
		instance->m_startFuction = startFunction;
		instance->m_object = obj;

		return Ref<ComponentScriptInstance>(instance);
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
		if (r < 0) {
			DEER_SCRIPT_INFO("Please correct the errors in the script and try again.");
		}
	}
}