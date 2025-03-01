#pragma once
#include "ScriptAttributes.h"

#include <string>

class asITypeInfo;
class asIScriptObject;
class asIScriptFunction;
class asIScriptContext;

namespace Deer {
	class ScriptEngine;

	class ComponentScriptInstance {
	public:
		ComponentScriptInstance() = default;
		~ComponentScriptInstance();

		void updateInternalVars();
		void start();
	private:
		asIScriptObject* m_object;
		asIScriptFunction* m_updateFunction;
		asIScriptFunction* m_startFuction;

		friend ScriptEngine;
	};

	class ComponentScript {
	public:
		ComponentScript() : m_typeInfo(nullptr) { }
		ComponentScript(asITypeInfo* typeInfo);

		inline const std::string& getName() { return m_scriptID; }
		inline const ScriptAttributeMap& getAttributes() { return m_attributes; }
		inline const ScriptAttribute getAttribute(const std::string& attributeID) { return m_attributes[attributeID]; }
	private:
		asITypeInfo* m_typeInfo;

		ScriptAttributeMap m_attributes;
		std::string m_scriptID;

		friend ScriptEngine;
	};
}

