#include "DeerScript.h"
#include "angelscript.h"

#include "Deer/Core/Log.h"

namespace Deer {
	DeerScript::DeerScript(asITypeInfo* typeInfo)
		: m_typeInfo(typeInfo) { 
		extractPublicAttributes();
	}

	const char* DeerScript::getName() {
		return m_typeInfo->GetName();
	}

	void DeerScript::extractPublicAttributes() {

		int atributes = m_typeInfo->GetPropertyCount();
		for (int x = 0; x < atributes; x++) {

			const char* name;
			int typeID;
			bool isPrivate;

			m_typeInfo->GetProperty(x, &name, &typeID, &isPrivate);

			DEER_SCRIPT_INFO("{0} type : {1} isPrivate : {2}", name, typeID, isPrivate);
		}

	}

	ScriptInstance DeerScript::createInstance() {
		return ScriptInstance();
	}
}