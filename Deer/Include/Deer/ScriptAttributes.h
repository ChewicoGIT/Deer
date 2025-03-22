#pragma once
#include <string>
#include <unordered_map>

class asITypeInfo;

namespace Deer {
	struct ScriptAttribute {
		std::string name;
		int typeID;
		int location;
		int internalID;
		bool isPrivate;

		ScriptAttribute(const char* _name = "", int _type = 0, bool _isPrivate = 0, int _location = 0, int _internalID = 0)
			: typeID(_type), name(_name), isPrivate(_isPrivate), location(_location), internalID(_internalID) { }
	};

	using ScriptAttributeMap = std::unordered_map<std::string, ScriptAttribute>;

	namespace Utils {
		ScriptAttributeMap extractAttributes(asITypeInfo* typeInfo);
	}
}