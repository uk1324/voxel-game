#version 430 core

uniform samplerCube skybox;

in vec3 texturePos;

out vec4 outColor;

void main()
{    
    outColor = texture(skybox, texturePos);
}