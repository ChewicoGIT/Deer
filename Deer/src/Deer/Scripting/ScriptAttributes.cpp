#include "ScriptAttributes.h"
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

			DEER_CORE_TRACE("{4} : {0} {1} isP : {2} {3}", typeID, name, isPrivate, offset, typeInfo->GetName());
			m_attributes.insert({ std::string(name), attribute});
		}

		return m_attributes;
	}
}