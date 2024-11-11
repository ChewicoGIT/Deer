#type vertex
#version 410 core
layout(location = 0) in uint a_vertexData;

uniform mat4 u_viewMatrix;
uniform mat4 u_worldMatrix;

out vec2 uv;
flat out uint normal;

void main() {

	uint xPos = (a_vertexData >> 25) & 31;
	uint yPos = (a_vertexData >> 20) & 31;
	uint zPos = (a_vertexData >> 15) & 31;

	normal = (a_vertexData >> 12) & 7;

	uint xUV = (a_vertexData >> 6) & 31;
	uint yUV = (a_vertexData >> 0) & 31;


	if (normal == 1)
		xPos += 1;

	if (normal == 3)
		yPos += 1;
		
	if (normal == 5)
		zPos += 1;

	uv = vec2(xUV, yUV);
	gl_Position = u_viewMatrix * u_worldMatrix * vec4(xPos, yPos, zPos, 1.0);
}

#type fragment
#version 410 core	

layout(location = 0) out vec4 fragColor;
layout(location = 1) out int objectID;

in vec2 uv;
flat in uint normal;

uniform int u_objectID;
uniform sampler2D u_texture;

vec3 normalDir[6] = vec3[6](
	vec3(-1, 0, 0),
	vec3( 1, 0, 0),
	vec3( 0, -1, 0),
	vec3( 0, 1, 0),
	vec3( 0, 0, -1),
	vec3( 0, 0, 1)
);

void main()
{
	vec3 normalVec = normalDir[normal];
	float light = dot(normalVec, normalize(vec3(3, 7, 5)));
	if (light < 0.2)
		light = 0.2;

	fragColor = texture(u_texture, uv) * light;
    objectID = u_objectID;
}
