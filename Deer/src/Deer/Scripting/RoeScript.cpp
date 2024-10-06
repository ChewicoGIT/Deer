#include "RoeScript.h"
#include "angelscript.h"

namespace Deer {
	const char* RoeScript::getClassName() {
		return m_typeInfo->GetName();
	}

	const char* RoeScript::getNameSpace() {
		return m_typeInfo->GetNamespace();
	}

	RoeScript::RoeScript(asITypeInfo* typeInfo)
		: m_typeInfo(typeInfo){

	}

}