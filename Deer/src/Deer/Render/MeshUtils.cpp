#include "MeshUtils.h"
#include "Buffer.h"

namespace Deer {
    Ref<Mesh> MeshUtils::createMesh(int vertexCount, float* vertices, int indexCount, unsigned int* indices) {

        Ref<VertexBuffer> vBuffer = VertexBuffer::create(vertices, vertexCount * sizeof(float));
        Ref<IndexBuffer> iBuffer = IndexBuffer::create(indices, indexCount * sizeof(unsigned int), IndexDataType::Unsigned_Int);

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
