#version 410 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec3 a_normal;

out vec2 textCoord;
out vec3 worldNormal;

uniform mat4 u_viewMatrix;
uniform mat4 u_worldMatrix;

void main()
{
	//gl_Position = vec4(a_position, 1.0) * u_projectionMatrix * u_viewMatrix;
	gl_Position = u_viewMatrix * u_worldMatrix * vec4(a_position,1.0);

	mat3 normalMatrix = transpose(inverse(mat3(u_worldMatrix)));
	vec3 transformedNormal = normalize(normalMatrix * a_normal);

	worldNormal = transformedNormal;
    textCoord = a_uv;
}
