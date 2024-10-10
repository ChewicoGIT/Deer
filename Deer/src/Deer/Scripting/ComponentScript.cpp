#include "ComponentScript.h"
#include "angelscript.h"

#include "Deer/Core/Log.h"
#include "Deer/Core/Project.h"

namespace Deer {
	ComponentScript::ComponentScript(asITypeInfo* typeInfo)
		: m_typeInfo(typeInfo) { 

		m_scriptID = m_typeInfo->GetName();
		m_attributes = Utils::extractAttributes(typeInfo);
	}

	void ComponentScriptInstance::update() {
		if (!m_updateFunction)
			return;

		asIScriptContext* context = Project::m_scriptEngine.getExecutionContext();
		context->Prepare(m_updateFunction);
		context->SetObject(m_object);

		context->Execute();
	}
}