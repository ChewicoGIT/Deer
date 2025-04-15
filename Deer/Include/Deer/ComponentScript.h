#pragma once
#include <string>
#include <unordered_map>

class asITypeInfo;
class asIScriptObject;
class asIScriptFunction;
class asIScriptContext;

namespace Deer {
	struct ScriptAttribute;
	using ScriptAttributeMap = std::unordered_map<std::string, ScriptAttribute>;

	struct ScriptAttribute {
		std::string name;
		int typeID;
		int location;
		int internalID;
		bool isPrivate;

		ScriptAttribute(const char* _name = "", int _type = 0,
		                bool _isPrivate = 0, int _location = 0,
		                int _internalID = 0)
		    : typeID(_type),
		      name(_name),
		      isPrivate(_isPrivate),
		      location(_location),
		      internalID(_internalID) {}
	};

	class ComponentScriptInstance {
	   public:
		ComponentScriptInstance() = default;
		~ComponentScriptInstance();

		void updateInternalVars();
		void start();

		asIScriptObject* m_object;
		asIScriptFunction* m_updateFunction;
		asIScriptFunction* m_startFuction;
	};

	class ComponentScript {
	   public:
		ComponentScript() : m_typeInfo(nullptr) {}
		ComponentScript(asITypeInfo* typeInfo);

		inline const std::string& getName() { return m_scriptID; }
		inline const ScriptAttributeMap& getAttributes() {
			return m_attributes;
		}
		inline const ScriptAttribute getAttribute(
		    const std::string& attributeID) {
			return m_attributes[attributeID];
		}

		inline asITypeInfo* getTypeInfo() { return m_typeInfo; }

	   private:
		asITypeInfo* m_typeInfo;

		ScriptAttributeMap m_attributes;
		std::string m_scriptID;
	};

	ScriptAttributeMap extractAttributes(asITypeInfo* typeInfo);
}  // namespace Deer
