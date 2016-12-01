#version 330 core

uniform mat4 transform;

layout(location = 0) in vec2 vertexPos;
layout(location = 1) in vec2 vertexUV;

out vec2 fragUV;

void main()
{
	gl_Position = transform * vec4(vertexPos, 0, 0);
  fragUV = vertexUV;
}