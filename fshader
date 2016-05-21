#version 450 core

in vec2 vTexCoord;
out vec4 color;

uniform sampler2D brickTexture;
uniform sampler2D faceTexture;
uniform float mixValue;

void main()
{
    color = mix(texture(brickTexture, vTexCoord), texture(faceTexture, vTexCoord), mixValue);
}