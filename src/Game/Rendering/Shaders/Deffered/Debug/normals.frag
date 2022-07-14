#version 430 core

uniform sampler2D geometryNormal;

in vec2 texturePos;

out vec4 outColor;

void main() 
{
    vec3 normal = normalize(texture(geometryNormal, texturePos).xyz);
    outColor = vec4((normal + 1) / 2, 1);
}