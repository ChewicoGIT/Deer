#pragma once

#include <string>

class asITypeInfo;

namespace Deer {
	class ScriptEngine;

	class RoeScript {
	public:
		RoeScript(asITypeInfo* typeInfo);

		const char* getClassName();
		const char* getNameSpace();
	private:
		asITypeInfo* m_typeInfo;

		friend ScriptEngine;
	};
}

