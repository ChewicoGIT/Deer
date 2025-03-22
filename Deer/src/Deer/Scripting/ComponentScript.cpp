#include "Deer/ComponentScript.h"
#include "angelscript.h"

#include "Deer/Core/Log.h"
#include "Deer/ScriptEngine.h"

namespace Deer {
	ComponentScript::ComponentScript(asITypeInfo* typeInfo)
		: m_typeInfo(typeInfo) { 

		m_scriptID = m_typeInfo->GetName();
		m_attributes = Utils::extractAttributes(typeInfo);
	}

	ComponentScriptInstance::~ComponentScriptInstance() {
		m_object->Release();
	}

	void ComponentScriptInstance::updateInternalVars() {
		if (!m_updateFunction)
			return;

		asIScriptContext* context = ScriptEngine::getExecutionContext();

		context->Prepare(m_updateFunction);
		context->SetObject(m_object);
		context->Execute();
	}

	void ComponentScriptInstance::start() {
		if (!m_startFuction)
			return;

		asIScriptContext* context = ScriptEngine::getExecutionContext();

		context->Prepare(m_startFuction);
		context->SetObject(m_object);
		context->Execute();
	}
}