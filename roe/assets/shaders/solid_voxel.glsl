#type vertex
#version 410 core
layout(location = 0) in int a_textureID;
layout(location = 1) in float a_ambient_occlusion;

layout(location = 2) in float a_xPos;
layout(location = 3) in float a_yPos;
layout(location = 4) in float a_zPos;

layout(location = 5) in float a_a_light;
layout(location = 6) in float a_r_light;
layout(location = 7) in float a_g_light;
layout(location = 8) in float a_b_light;

layout(location = 9) in int a_normal;

layout(location = 10) in float a_u;
layout(location = 11) in float a_v;

uniform mat4 u_viewMatrix;
uniform mat4 u_worldMatrix;

uniform int u_chunkID_x;
uniform int u_chunkID_y;
uniform int u_chunkID_z;

uniform int u_textureSize;

out vec2 uv;
out vec3 voxel_light;
out float ambient_light;
out float ambient_occlusion;

flat out uint normal;

void main() {
	voxel_light = vec3(a_r_light, a_g_light, a_b_light);
	ambient_light = a_a_light;
	ambient_occlusion = a_ambient_occlusion;

	normal = a_normal;
	
	int posY = a_textureID / u_textureSize;
	int posX = a_textureID - posY * u_textureSize;

	uv = vec2((a_u + float(posX)) / float(u_textureSize), (a_v + float(posY)) / float(u_textureSize));

	gl_Position = u_viewMatrix * u_worldMatrix * vec4(a_xPos + u_chunkID_x * 32, a_yPos + u_chunkID_y * 32, a_zPos + u_chunkID_z * 32, 1.0);
}

#type fragment
#version 410 core	

layout(location = 0) out vec4 fragColor;
layout(location = 1) out int objectID;

in vec2 uv;
in vec3 voxel_light;
in float ambient_light;
in float ambient_occlusion;

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
    objectID = -1;

	vec3 normalVec = normalDir[normal];

	vec3 tLight = vec3(ambient_light, ambient_light, ambient_light) + voxel_light;
	tLight = vec3(clamp(tLight.r / 255, 0, 1), clamp(tLight.g / 255, 0, 1), clamp(tLight.b / 255, 0, 1));

	vec4 col = texture(u_texture, uv) * vec4(tLight, 1);
	fragColor = (col * 3) / (3 + ambient_occlusion);
}
