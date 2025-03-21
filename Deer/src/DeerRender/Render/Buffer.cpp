#include "Buffer.h"

#include "Deer/Core/Log.h"
#include "DeerRender/Render/Render.h"

namespace Deer {
    unsigned int dataTypeSize(DataType type) {
		switch (type) {
		case DataType::None: DEER_CORE_ERROR("Unkown shader data type"); return 0;
			// Halfs : 2
		case DataType::Half:	return 2;
		case DataType::Half2:	return 2 * 2;
		case DataType::Half3:	return 2 * 3;
		case DataType::Half4: return 2 * 4;;
			// Floats : 4
		case DataType::Float:	 return 4;
		case DataType::Float2: return 4 * 2;
		case DataType::Float3: return 4 * 3;
		case DataType::Float4: return 4 * 4;
			// Byte : 1
		case DataType::Byte:	return 1;
		case DataType::Byte2:	return 1 * 2;
		case DataType::Byte3:	return 1 * 3;
		case DataType::Byte4: return 1 * 4;
			// Short : 2
		case DataType::Short:	 return 2;
		case DataType::Short2: return 2 * 2;
		case DataType::Short3: return 2 * 3;
		case DataType::Short4: return 2 * 4;
			// Ints : 4
		case DataType::Int:  return 4;
		case DataType::Int2: return 4 * 2;
		case DataType::Int3: return 4 * 3;
		case DataType::Int4: return 4 * 4;
			// Byte : 1
		case DataType::Unsigned_Byte:	 return 1;
		case DataType::Unsigned_Byte2: return 1 * 2;
		case DataType::Unsigned_Byte3: return 1 * 3;
		case DataType::Unsigned_Byte4: return 1 * 4;
			// Short : 2
		case DataType::Unsigned_Short:  return 2;
		case DataType::Unsigned_Short2: return 2 * 2;
		case DataType::Unsigned_Short3: return 2 * 3;
		case DataType::Unsigned_Short4: return 2 * 4;
			// Ints : 4
		case DataType::Unsigned_Int:	return 4;
		case DataType::Unsigned_Int2:	return 4 * 2;
		case DataType::Unsigned_Int3:	return 4 * 3;
		case DataType::Unsigned_Int4: return 4 * 4;
		}

		DEER_CORE_ERROR("Unkown shader data type");
	}

	unsigned int dataTypeCount(DataType type) {
		switch (type) {
		case DataType::None: DEER_CORE_ERROR("Unkown shader data type"); return 0;
			// Halfs : 2
		case DataType::Half:	return 1;
		case DataType::Half2:	return 2;
		case DataType::Half3:	return 3;
		case DataType::Half4: return 4;
			// Floats : 4
		case DataType::Float: return 1;
		case DataType::Float2: return 2;
		case DataType::Float3: return 3;
		case DataType::Float4: return 4;
			// Byte : 1
		case DataType::Byte:	return 1;
		case DataType::Byte2:	return 2;
		case DataType::Byte3:	return 3;
		case DataType::Byte4: return 4;
			// Short : 2
		case DataType::Short:	 return 1;
		case DataType::Short2: return 2;
		case DataType::Short3: return 3;
		case DataType::Short4: return 4;
			// Ints : 4
		case DataType::Int:  return 1;
		case DataType::Int2: return 2;
		case DataType::Int3: return 3;
		case DataType::Int4: return 4;
			// Byte : 1
		case DataType::Unsigned_Byte:	 return 1;
		case DataType::Unsigned_Byte2: return 2;
		case DataType::Unsigned_Byte3: return 3;
		case DataType::Unsigned_Byte4: return 4;
			// Short : 2
		case DataType::Unsigned_Short:  return 1;
		case DataType::Unsigned_Short2: return 2;
		case DataType::Unsigned_Short3: return 3;
		case DataType::Unsigned_Short4: return 4;
			// Ints : 4
		case DataType::Unsigned_Int:	return 1;
		case DataType::Unsigned_Int2:	return 2;
		case DataType::Unsigned_Int3:	return 3;
		case DataType::Unsigned_Int4: return 4;
		}

		DEER_CORE_ERROR("Unkown shader data type");
	}

	unsigned int indexDataTypeSize(IndexDataType type)
	{
		switch (type) {
		case IndexDataType::None: DEER_CORE_ERROR("Unkown shader data type"); return 0;

		case IndexDataType::Unsigned_Byte:	return 1;
		case IndexDataType::Unsigned_Short:	return 2;
		case IndexDataType::Unsigned_Int:	return 4;
		}

		DEER_CORE_ERROR("Unkown shader data type");
	}

	void BufferLayout::calculateOffsetAndStride() {
		unsigned int offset = 0;
		int calc_stride = 0;
		for (auto& element : m_bufferElements) {
			if (element.offset == -1)
				element.offset = offset;
			offset += dataTypeSize(element.type);
			calc_stride += dataTypeSize(element.type);
		}

		if (m_stride == -1)
			m_stride = calc_stride;
	}
}
