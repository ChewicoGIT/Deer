#include "RenderUtils.h"
#include "Deer/Render/VertexArray.h"
#include "Deer/Render/Buffer.h"
#include "Deer/Render/Shader.h"

namespace Deer {
	Ref<VertexArray> RenderUtils::m_lineVertexArray;
	Ref<Shader> RenderUtils::m_lineShader;

	void RenderUtils::initializeRenderUtils() {
		m_lineVertexArray = genLineVertexArray();
		m_lineShader = getLineShader();
	}

	Ref<VertexArray> RenderUtils::genLineVertexArray() {
		unsigned int vertices[2] = { 0, 1 };

		Ref<VertexArray> vertexArray = VertexArray::create();
		Ref<VertexBuffer> vertexBuffer = VertexBuffer::create(vertices, sizeof(vertices));
		Ref<IndexBuffer> indexBuffer = IndexBuffer::create(vertices, sizeof(vertices), IndexDataType::Unsigned_Int);
		
		BufferLayout layout({
			{"a_vertexID", DataType::Unsigned_Int, ShaderDataType::Integer}
			});

		vertexBuffer->setLayout(layout);

		vertexArray->addVertexBuffer(vertexBuffer);
		vertexArray->setIndexBuffer(indexBuffer);

		return vertexArray;
	}

	Ref<Shader> RenderUtils::getLineShader() {
		std::string vertexSrc = R"(
#version 410 core

layout(location = 0) in int a_vertexID;

uniform mat4 u_viewMatrix;
uniform vec3 u_posA;
uniform vec3 u_posB;

void main()
{

	vec3 pos;
	if (a_vertexID == 0) {
		pos = u_posA;
	} else {
		pos = u_posB;
	}
	
	gl_Position = u_viewMatrix * vec4(pos,1.0);
})";

		std::string fragmentSrc = R"(
#version 410 core	

layout(location = 0) out vec4 fragColor;

uniform vec3 u_color;

void main()
{
	fragColor = vec4(u_color, 1.0);
})";

		return Shader::create(vertexSrc, fragmentSrc);
	}
}