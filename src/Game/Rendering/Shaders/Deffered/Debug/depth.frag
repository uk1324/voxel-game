#version 430 core

uniform sampler2D geometryDepth;
uniform float nearPlaneZ;
uniform float farPlaneZ;

in vec2 texturePos;

out vec4 outColor;

// Perspective projection transforms depth non-linearly. This reverses this transformation and the puts the values into range [0, 1]
float transformDepth(float depth)
{
    // Fron NDC to [0, 1].
    depth = 2.0 * depth - 1.0;
    return (2.0 * nearPlaneZ * farPlaneZ / (farPlaneZ + nearPlaneZ - depth * (farPlaneZ - nearPlaneZ))) / farPlaneZ;
}

void main()
{
    float depth = transformDepth(texture(geometryDepth, texturePos).r);
    outColor = vec4(vec3(depth), 1);
}