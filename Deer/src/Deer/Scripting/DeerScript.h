#pragma once

#include <string>
#include "Deer/Scripting/ScriptInstance.h"

class asITypeInfo;

namespace Deer {
	class ScriptEngine;

	class DeerScript {
	public:
		DeerScript(asITypeInfo* typeInfo);

		const char* getName();
	private:
		void extractPublicAttributes();

		ScriptInstance createInstance();
		asITypeInfo* m_typeInfo;

		friend ScriptEngine;
	};
}

