#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform float time;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
	float offset = (gl_Position.x + (gl_Position.z * 0.2)) * 0.5;
	gl_Position.y += sin(offset + time) * 0.2;
}