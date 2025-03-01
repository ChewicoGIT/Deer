#type vertex
#version 410 core
layout(location = 0) in float a_xPos;
layout(location = 1) in float a_yPos;
layout(location = 2) in float a_zPos;
layout(location = 3) in uint a_normal;
layout(location = 4) in float a_u;
layout(location = 5) in float a_v;
layout(location = 6) in float a_ambient_light;

uniform mat4 u_viewMatrix;
uniform mat4 u_worldMatrix;

uniform int u_chunkID_x;
uniform int u_chunkID_y;
uniform int u_chunkID_z;

out vec2 uv;
out float ambient_light;
flat out uint normal;

void main() {

	normal = a_normal;
	uv = vec2(a_u, a_v);
	ambient_light = a_ambient_light;
	gl_Position = u_viewMatrix * u_worldMatrix * vec4(a_xPos + u_chunkID_x * 32, a_yPos + u_chunkID_y * 32, a_zPos + u_chunkID_z * 32, 1.0);
}

#type fragment
#version 410 core	

layout(location = 0) out vec4 fragColor;
layout(location = 1) out int objectID;

in vec2 uv;
in float ambient_light;
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
	float light = ambient_light / 255;
	if (light < 0.1)
		light = 0.1;

	fragColor = texture(u_texture, uv) * light;
    objectID = -1;
}
