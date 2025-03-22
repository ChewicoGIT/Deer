#pragma once
#include "Deer/Memory.h"

#include <vector>
#include <string>

namespace Deer {

	enum class ShaderDataType {
		None = 0,
		FloatingPoint,
		NormalizedFloatingPoint,
		Integer
	};

	enum class DataType {
		None = 0,
		Half, Half2, Half3, Half4,
		Float, Float2, Float3, Float4,

		Byte, Byte2, Byte3, Byte4,
		Short, Short2, Short3, Short4,
		Int, Int2, Int3, Int4,

		Unsigned_Byte, Unsigned_Byte2, Unsigned_Byte3, Unsigned_Byte4,
		Unsigned_Short, Unsigned_Short2, Unsigned_Short3, Unsigned_Short4,
		Unsigned_Int, Unsigned_Int2, Unsigned_Int3, Unsigned_Int4
	};

	enum class IndexDataType {
		None = 0,
		Unsigned_Byte,
		Unsigned_Short,
		Unsigned_Int,
	};

	unsigned int dataTypeSize(DataType type);
	unsigned int dataTypeCount(DataType type);
	unsigned int indexDataTypeSize(IndexDataType type);

	struct BufferElement {
		std::string name;
		DataType type;
		ShaderDataType shaderType;
		int offset;

		BufferElement(std::string _name, DataType _type , ShaderDataType _shaderType = ShaderDataType::FloatingPoint,
			int _offset = -1)
			: name(_name), type(_type), shaderType(_shaderType), offset(_offset){
		}
	};

	class BufferLayout {
	public:
		BufferLayout() { }
		BufferLayout(const std::initializer_list<BufferElement>& elements, int _stride = -1)
			 : m_bufferElements(elements), m_stride(_stride) {
				calculateOffsetAndStride();
		}
		BufferLayout(const std::vector<BufferElement> elements)
			: m_bufferElements(elements) {
			calculateOffsetAndStride();
		}

		inline std::vector<BufferElement>::iterator begin() { return m_bufferElements.begin(); }
		inline std::vector<BufferElement>::iterator end() { return m_bufferElements.end(); }
		inline std::vector<BufferElement> getElements() { return m_bufferElements; }
		inline int getStride() { return m_stride; }
	private:
		void calculateOffsetAndStride();
		
		std::vector<BufferElement> m_bufferElements;
		int m_stride;
	};

	class VertexBuffer {
	public:
		virtual ~VertexBuffer() = default;

		virtual void bind() const = 0;
		virtual void setLayout(BufferLayout& layout) = 0;
		virtual BufferLayout& getLayout() = 0;
		
		static Ref<VertexBuffer> create(void* data, unsigned int size);
	};

	class IndexBuffer {
	public:
		virtual ~IndexBuffer() = default;

		virtual void bind() const = 0;
		virtual unsigned int getCount() const = 0;
		virtual IndexDataType getIndexDataType() const = 0;

		static Ref<IndexBuffer> create(void* data, unsigned int size, IndexDataType indexDataType);
	};
}

