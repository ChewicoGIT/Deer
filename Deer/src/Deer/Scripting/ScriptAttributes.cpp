#include "Deer/ScriptAttributes.h"
#include "Deer/Core/Log.h"
#include "angelscript.h"

namespace Deer {
	ScriptAttributeMap Utils::extractAttributes(asITypeInfo* typeInfo) {
		ScriptAttributeMap m_attributes;

		int atributes = typeInfo->GetPropertyCount();
		for (int x = 0; x < atributes; x++) {
			const char* name;
			bool isPrivate;
			int typeID;
			int offset;

			typeInfo->GetProperty(x, &name, &typeID, &isPrivate, nullptr, &offset);
			ScriptAttribute attribute(name, typeID, isPrivate, offset, x);

			m_attributes.insert({ std::string(name), attribute});
		}

		return m_attributes;
	}
}