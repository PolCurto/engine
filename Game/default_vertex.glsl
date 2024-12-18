#version 460

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv0;
layout(location = 2) in vec3 vertex_normals;

layout(location = 0) uniform mat4 projection;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 model;

out vec2 uv0;
out vec3 position_normal;
out vec3 fragment_position;

void main()
{
	gl_Position = projection * view * model * vec4(vertex_position, 1.0f);
	uv0 = vertex_uv0;
	fragment_position = (model * vec4(vertex_position, 1.0)).xyz;
	position_normal = transpose(inverse(mat3(model))) * vertex_normals;
}