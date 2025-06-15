#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;
layout(location = 3) in vec4 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool color_enabled = false;

out vec4 VertexColor;

void main() 
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    VertexColor = aColor * vec4(color_enabled) + vec4(0.5, 0.5, 0.5, 1.0) * vec4(!color_enabled);
}
