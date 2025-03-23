#include "RenderUtils.h"
#include "DeerRender/Render/VertexArray.h"
#include "DeerRender/Render/Buffer.h"
#include "DeerRender/Render/Shader.h"

#ifdef DEER_RENDER

namespace Deer {
	namespace RenderUtils {
		Ref<VertexArray> m_lineVertexArray;
		Ref<VertexArray> m_faceVertexArray;
		Ref<Shader> m_lineShader;
		Ref<Shader> m_faceShader;

		Ref<VertexArray> genLineVertexArray();
		Ref<VertexArray> genFaceVertexArray();
		Ref<Shader> getLineShader();
		Ref<Shader> getFaceShader();
	}

	void RenderUtils::initializeRenderUtils() {
		m_lineVertexArray = genLineVertexArray();
		m_lineShader = getLineShader();
		m_faceVertexArray = genFaceVertexArray();
		m_faceShader = getFaceShader();
	}

	Ref<VertexArray> RenderUtils::genLineVertexArray() {
		unsigned int vertices[2] = { 0, 1 };

		Ref<VertexArray> vertexArray = VertexArray::create();
		vertexArray->bind();
		
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
layout(location = 1) out int objectID;

uniform vec3 u_color;

void main()
{
	objectID = -1;
	fragColor = vec4(u_color, 1.0);
})";

		return Shader::create(vertexSrc, fragmentSrc);
	}

	Ref<VertexArray> RenderUtils::genFaceVertexArray() {
		unsigned int vertices[4] = {
			0, 1, 2, 3
		};
		unsigned int indices[6] = {
			0, 2, 1, 1, 2, 3
		};

		Ref<VertexArray> vertexArray = VertexArray::create();
		vertexArray->bind();

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::create(vertices, sizeof(vertices));
		Ref<IndexBuffer> indexBuffer = IndexBuffer::create(indices, sizeof(indices), IndexDataType::Unsigned_Int);

		BufferLayout layout({
			{"a_vertexID", DataType::Unsigned_Int, ShaderDataType::Integer}
		});

		vertexBuffer->setLayout(layout);
		vertexArray->addVertexBuffer(vertexBuffer);
		vertexArray->setIndexBuffer(indexBuffer);

		return vertexArray;
	}

	Ref<Shader> RenderUtils::getFaceShader() {
		std::string vertexSrc = R"(
#version 410 core

layout(location = 0) in int a_vertexID;

uniform mat4 u_viewMatrix;
uniform vec3 u_posA;
uniform vec3 u_posB;
uniform vec3 u_posC;
uniform vec3 u_posD;

uniform int u_textureSize;
uniform int u_textureID;

out vec2 uv;

void main()
{
	vec3 pos;
	vec2 a_uv;
	if (a_vertexID == 0) {
		pos = u_posA;
		a_uv = vec2(0, 0);
	} else if (a_vertexID == 1) {
		pos = u_posB;
		a_uv = vec2(1, 0);
	} else if (a_vertexID == 2) {
		pos = u_posC;
		a_uv = vec2(0, 1);
	} else {
		pos = u_posD;
		a_uv = vec2(1, 1);
	}
	
	int posY = u_textureID / u_textureSize;
	int posX = u_textureID - posY * u_textureSize;

	uv = vec2((a_uv.x + float(posX)) / float(u_textureSize), (a_uv.y + float(posY)) / float(u_textureSize));

	gl_Position = u_viewMatrix * vec4(pos,1.0);
})";

		std::string fragmentSrc = R"(
#version 410 core	

layout(location = 0) out vec4 fragColor;
layout(location = 1) out int objectID;

in vec2 uv;

uniform sampler2D u_texture;

void main()
{
	vec4 color = texture(u_texture, uv);
	if (color.a == 0){
		discard;
		return;
	}
	
	objectID = -1;
	fragColor = color;
})";

		return Shader::create(vertexSrc, fragmentSrc);
	}

}
#endif 
