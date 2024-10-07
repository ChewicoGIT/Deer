#pragma once

#include <string>
#include "Deer/Scripting/RoeInstance.h"

class asITypeInfo;

namespace Deer {
	class ScriptEngine;

	class RoeScript {
	public:
		RoeScript(asITypeInfo* typeInfo);

		const char* getClassName();
		const char* getNameSpace();
	private:
		RoeInstance createInstance();

		asITypeInfo* m_typeInfo;

		friend ScriptEngine;
	};
}

