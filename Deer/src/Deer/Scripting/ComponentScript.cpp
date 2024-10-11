#include "ComponentScript.h"
#include "angelscript.h"

#include "Deer/Core/Log.h"
#include "Deer/Core/Project.h"
#include "Deer/Scripting/ScriptEngine.h"

namespace Deer {
	ComponentScript::ComponentScript(asITypeInfo* typeInfo)
		: m_typeInfo(typeInfo) { 

		m_scriptID = m_typeInfo->GetName();
		m_attributes = Utils::extractAttributes(typeInfo);
	}

	ComponentScriptInstance::~ComponentScriptInstance() {
		m_object->Release();
	}

	void ComponentScriptInstance::update() {
		if (!m_updateFunction)
			return;

		asIScriptContext* context = Project::m_scriptEngine->getExecutionContext();

		context->Prepare(m_updateFunction);
		context->SetObject(m_object);
		context->Execute();
	}
}