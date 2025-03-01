#include "MeshUtils.h"
#include "Buffer.h"
#include "VertexArray.h"

namespace Deer {
    Ref<VertexArray> MeshUtils::createMesh(int vertexCount, float* vertices, int indexCount, unsigned int* m_indices) {

        Ref<VertexBuffer> vBuffer = VertexBuffer::create(vertices, vertexCount * sizeof(float));
        Ref<IndexBuffer> iBuffer = IndexBuffer::create(m_indices, indexCount * sizeof(unsigned int), IndexDataType::Unsigned_Int);

        BufferLayout layout({
            {"a_Position", DataType::Float3, ShaderDataType::FloatingPoint}
        });
        vBuffer->setLayout(layout);

        Ref<VertexArray> vertexArray = VertexArray::create();
        vertexArray->addVertexBuffer(vBuffer);
        vertexArray->setIndexBuffer(iBuffer);

        return vertexArray;
    }
}
