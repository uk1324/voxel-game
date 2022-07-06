#version 430 core

uniform sampler2D geometryPosWorld;
uniform sampler2D geometryAlbedo;
uniform sampler2D geometryNormal;
uniform sampler2D geometryDepth;
uniform sampler2D shadowMaps[16];
uniform mat4 worldToShadowMap[16];
uniform float cascadeZ[16];
uniform int cascadeCount;

in vec2 texturePos;

out vec4 outColor;

float linearizeDepth(float depth, float near, float far) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return ((2.0 * near * far) / (far + near - z * (far - near))) / far;
}

uniform int test;

vec3 directionalLightDir = normalize(vec3(-0.5, -1, -0.5));

void main() 
{
    vec3 color = texture2D(geometryAlbedo, texturePos).rgb;
    vec4 posWorld = vec4(texture2D(geometryPosWorld, texturePos).xyz, 1);
    vec3 normal = texture(geometryNormal, texturePos).xyz;
     
    vec4 posLightSpaceNotProjected = worldToShadowMap[0] * posWorld;
    vec3 posLightSpace = posLightSpaceNotProjected.xyz / posLightSpaceNotProjected.w;
    posLightSpace = posLightSpace * 0.5 + 0.5;
    
    float closestDepth = texture2D(shadowMaps[0], posLightSpace.xy).r; 
    float currentDepth = posLightSpace.z;
    
    float bias = 0.0001;
    // float bias = max(0.05 * (1.0 - dot(normal, directionalLightDir)), 0.005);
    if (currentDepth - bias > closestDepth)
    {
        color *= vec3(0.2);
    }

    if (test == 1) 
    {
        // outColor = vec4(vec3(closestDepth), 1);
        outColor = vec4(vec3(texture(shadowMaps[0], texturePos).r), 1);
    }
    else if (test == 2)
    {
        outColor = vec4(vec3(texture(geometryNormal, texturePos).xyz), 1);
        // outColor = vec4(posLightSpace, 1);
    }
    else 
    {
        outColor = vec4(color, 1);
    }
//	vec3 color = texture2D(geometryAlbedo, texturePos).rgb;
//    vec4 posWorld = vec4(texture2D(geometryPosWorld, texturePos).rgb, 1);
//    vec4 posView = worldToView * posWorld;
//    vec4 lightSpacePosProj = toLightSpace * posWorld;
//    vec3 lightSpacePos = lightSpacePosProj.xyz / lightSpacePosProj.w;
//    lightSpacePos = lightSpacePos * 0.5 + 0.5;
//    float d1 = texture2D(geometryDepth, texturePos).r;
//    float d2 = texture2D(shadowMap[0], lightSpacePos.xy).r;
//    if (lightSpacePos.z > d1) 
//    {
//        color *= vec3(0.2);
//    }
//    outColor = vec4(color, 1);
//    // outColor = vec4(vec3(linearizeDepth(texture2D(shadowMap[0], texturePos).r, 0.1, 96)), 1);
    // outColor = vec4(vec3(texture2D(shadowMap[0], texturePos).r), 1);
}