#version 410

layout (location = 0) in vec3 position;
layout (location = 2) in vec3 normal;

uniform mat4 transform;

out vec3 fragNormal;

void main()
{
	gl_Position = transform * vec4(position, 1.0);
	fragNormal = normal;
}