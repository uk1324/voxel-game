#version 330 core

uniform samplerCube skybox;


in vec3 TexCoords;

layout (location = 0) out vec4 outPos;
layout (location = 1) out vec4 outNormal;
layout (location = 2) out vec4 outAlbedo;

void main()
{    
    outAlbedo = texture(skybox, TexCoords);
    outNormal = vec4(0);
    outPos = vec4(999999);
}