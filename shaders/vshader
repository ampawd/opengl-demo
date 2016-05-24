#version 450 core

layout (location = 0) in vec4 position;
layout (location = 2) in vec2 texCoord;

uniform mat4 mvp;
out vec2 vTexCoord;

void main()
{
    gl_Position = mvp * position;
    vTexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
}
