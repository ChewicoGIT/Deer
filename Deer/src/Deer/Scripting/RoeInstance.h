#pragma once

class asIScriptObject;
class asIScriptFunction;

namespace Deer {
	class ScriptEngine;

	class RoeInstance {
	public:
		RoeInstance() = default;

	private:
		asIScriptObject* m_object;
		asIScriptFunction* m_updateFunction;

		friend ScriptEngine;
	};
}

