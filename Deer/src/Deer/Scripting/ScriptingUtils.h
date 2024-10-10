#pragma once

#include <string>
#include <unordered_map>

class asITypeInfo;

namespace Deer {
	struct ScriptAttribute {
		std::string name;
		int typeID;
		int location;
		int positionID;
		bool isPrivate;

		ScriptAttribute(const char* _name = "", int _type = 0, bool _isPrivate = 0, int _location = 0, int _positionID = 0)
			: typeID(_type), name(_name), isPrivate(_isPrivate), location(_location), positionID(_positionID) { }
	};

	using ScriptAttributeMap = std::unordered_map<std::string, ScriptAttribute>;

	namespace Utils {
		ScriptAttributeMap extractAttributes(asITypeInfo* typeInfo);
	}
}