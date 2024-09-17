#include "Asset.h"
#include "Deer/Render/Texture.h"
#include "Deer/Render/Buffer.h"
#include "Deer/Render/VertexArray.h"

namespace Deer {
	TextureAsset::TextureAsset(uid id, const std::string& assetLocation)
		: Asset(id, assetLocation) { 
		
		m_texture = Texture2D::create(assetLocation);
	}

    // TODO: load from file
	MeshAsset::MeshAsset(uid id, const std::string& assetLocation) {
		Ref<VertexBuffer> m_vertexBuffer;
		Ref<IndexBuffer> m_indexBuffer;

        m_mesh = VertexArray::create();

        float vertices[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f
        };

        m_vertexBuffer = VertexBuffer::create(vertices, sizeof(vertices));

        BufferLayout bufferLayout({
            {"a_Position", DataType::Float3, ShaderDataType::FloatingPoint },
            {"a_Color", DataType::Float2, ShaderDataType::FloatingPoint },
            });

        m_vertexBuffer->setLayout(bufferLayout);
        m_mesh->addVertexBuffer(m_vertexBuffer);

        unsigned int indices[] = {
            0, 2, 1,
            1, 2, 3
        };

        m_indexBuffer = IndexBuffer::create(indices, sizeof(indices), IndexDataType::Unsigned_Int);
        m_mesh->setIndexBuffer(m_indexBuffer);
	}
}
