#version 410 core	

layout(location = 0) out vec4 fragColor;
layout(location = 1) out int objectID;

in vec2 textCoord;
in vec3 worldNormal;

uniform sampler2D u_texture;
uniform int u_objectID;

void main()
{
	float v = clamp(dot(worldNormal, normalize(vec3(1,2,3))), 0.1, 1.0);
	fragColor = texture(u_texture, textCoord) * v;
    objectID = u_objectID;
}