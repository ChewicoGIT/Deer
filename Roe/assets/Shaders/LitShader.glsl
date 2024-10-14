#type vertex
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

#type fragment
#version 410 core	

layout(location = 0) out vec4 fragColor;
layout(location = 1) out int objectID;

in vec2 textCoord;
in vec3 worldNormal;

uniform sampler2D u_texture;
uniform int u_objectID;

void main()
{
	fragColor = texture(u_texture, textCoord);
    objectID = u_objectID;
}
