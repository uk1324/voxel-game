#version 430 core

uniform sampler2D geometryAlbedo;

in vec2 texturePos;

out vec4 outColor;

void main() 
{
    outColor = texture(geometryAlbedo, texturePos);
}