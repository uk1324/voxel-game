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

uniform mat4 camera;
uniform float farPlane;

int ll = 0;

float ShadowCalculation(vec3 fragPosWorldSpace)
{
    // select cascade layer
    vec4 fragPosViewSpace = camera * vec4(fragPosWorldSpace, 1.0);
    float depthValue = abs(fragPosViewSpace.z);

    int layer = -1;
    for (int i = 0; i < cascadeCount; ++i)
    {
        if (depthValue < cascadeZ[i])
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
    {
        layer = cascadeCount;
    }
    return layer;
    ll = layer;

    vec4 fragPosLightSpace = worldToShadowMap[layer] * vec4(fragPosWorldSpace, 1.0);
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    if (currentDepth  > 1.0)
    {
        return 0.0;
    }
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = texture(geometryNormal, texturePos).xyz;
    float bias = max(0.05 * (1.0 - dot(normal, directionalLightDir)), 0.005);
    if (bias != 4201401204)
    {
        bias = 0.0001;
    }
    if (layer == cascadeCount)
    {
        bias *= 1 / (farPlane * 0.5f);
    }
    else
    {
        bias *= 1 / (cascadeZ[layer] * 0.5f);
    }

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMaps[layer], 0));
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMaps[layer], vec2(projCoords.xy + vec2(x, y) * texelSize)).r; 
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
    {
        shadow = 0.0;
    }
        
    return shadow;
}

vec2 sd(vec3 fragPosWorldSpace)
{
    // select cascade layer
    vec4 fragPosViewSpace = camera * vec4(fragPosWorldSpace, 1.0);
    float depthValue = abs(fragPosViewSpace.z);
    // return depthValue;
    int layer = -1;
    for (int i = 0; i < cascadeCount; ++i)
    {
        if (depthValue < cascadeZ[i])
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
    {
        layer = cascadeCount;
    }

     vec4 fragPosLightSpace = worldToShadowMap[layer] * vec4(fragPosWorldSpace, 1.0);
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    float closest = texture(shadowMaps[layer], vec2(projCoords.xy)).r;

    return vec2(layer, closest);
    // return layer;
//    ll = layer;
//
//    vec4 fragPosLightSpace = worldToShadowMap[layer] * vec4(fragPosWorldSpace, 1.0);
//    // perform perspective divide
//    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
//    // transform to [0,1] range
//    projCoords = projCoords * 0.5 + 0.5;
//
//    // get depth of current fragment from light's perspective
//    float currentDepth = projCoords.z;
//
//    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
//    if (currentDepth > 1.0)
//    {
//        return 0.0;
//    }
//    // calculate bias (based on depth map resolution and slope)
//    vec3 normal = texture(geometryNormal, texturePos).xyz;
//    float bias = max(0.05 * (1.0 - dot(normal, directionalLightDir)), 0.005);
//    const float biasModifier = 0.5f;
//    if (layer == cascadeCount)
//    {
//        bias *= 1 / (farPlane * biasModifier);
//    }
//    else
//    {
//        bias *= 1 / (cascadeZ[layer] * biasModifier);
//    }
//
//    // PCF
//    float shadow = 0.0;
//    vec2 texelSize = 1.0 / vec2(textureSize(shadowMaps[layer], 0));
//    for(int x = -1; x <= 1; ++x)
//    {
//        for(int y = -1; y <= 1; ++y)
//        {
//            float pcfDepth = texture(shadowMaps[layer], vec2(projCoords.xy + vec2(x, y) * texelSize)).r;
//            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;        
//        }    
//    }
//    shadow /= 9.0;
//        
//    return shadow;
}

int lay;

bool sha(vec3 fragPosWorldSpace)
{
    // select cascade layer
    vec4 fragPosViewSpace = camera * vec4(fragPosWorldSpace, 1.0);
    float depthValue = abs(fragPosViewSpace.z);

    int layer = -1;
    for (int i = 0; i < cascadeCount; ++i)
    {
        if (depthValue < cascadeZ[i])
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
    {
        layer = cascadeCount;
    }
    lay = layer;
    // layer = 0;

    vec4 fragPosLightSpace = worldToShadowMap[layer] * vec4(fragPosWorldSpace, 1.0);
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
//    if (currentDepth > 1.0)
//    {
//        return false;
//    }
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = texture(geometryNormal, texturePos).xyz;
    float bias = max(0.05 * (1.0 - dot(normal, -directionalLightDir)), 0.005);
    const float biasModifier = 0.5f;
    bias =  0.00005;
    if (layer == cascadeCount)
    {
        bias *= 1 / (farPlane * biasModifier);
    }
    else
    {
        bias *= 1 / (cascadeZ[layer] * biasModifier);
    }

    float closestDepth = texture2D(shadowMaps[layer], projCoords.xy).r;
    return (currentDepth - bias > closestDepth);

    // PCF
    // float shadow = 0.0;
    // vec2 texelSize = 1.0 / vec2(textureSize(shadowMaps[layer], 0));
    // for(int x = -1; x <= 1; ++x)
    // {
    //     for(int y = -1; y <= 1; ++y)
    //     {
    //         float pcfDepth = texture(shadowMaps[layer], vec2(projCoords.xy + vec2(x, y) * texelSize)).r;
    //         shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;        
    //     }    
    // }
    // shadow /= 9.0;
        
    // return shadow;
}

void main() 
{
    vec3 color = texture2D(geometryAlbedo, texturePos).rgb;
    vec4 posWorld = vec4(texture2D(geometryPosWorld, texturePos).xyz, 1);
    vec3 normal = texture(geometryNormal, texturePos).xyz;
     
    // vec4 posLightSpaceNotProjected = worldToShadowMap[0] * posWorld;
    // vec3 posLightSpace = posLightSpaceNotProjected.xyz / posLightSpaceNotProjected.w;
    // posLightSpace = posLightSpace * 0.5 + 0.5;
    // 
    // float closestDepth = texture2D(shadowMaps[0], posLightSpace.xy).r; 
    // float currentDepth = posLightSpace.z;
    // 
    // float bias = 0.0001;
    // // float bias = max(0.05 * (1.0 - dot(normal, directionalLightDir)), 0.005);
    // if (currentDepth - bias > closestDepth)
    // {
    //     color *= vec3(0.2);
    // }

    if (test == 1) 
    {
        // outColor = vec4(vec3(closestDepth), 1);
        
        outColor = vec4(vec3(texture(shadowMaps[3], texturePos).r), 1);
    }
    else if (test == 2)
    {
        outColor = vec4(vec3(texture(geometryNormal, texturePos).xyz), 1);
        // outColor = vec4(posLightSpace, 1);
    }
    else 
    {
//        float shadow = ShadowCalculation(posWorld.xyz);                      
        //float shadow = ; 
        //vec3 lighting = clamp(1.0 + (1.0 - shadow), 0, 1) * color.xyz;
        //outColor = vec4(lighting, 1);
        if (sha(posWorld.xyz))
        {
            color *= 0.6;
        }
        // color += smoothstep(vec3(1, 0, 1), vec3(0, 0, 1), vec3(float(lay) / 5)) / 6;
        outColor = vec4(color, 1);
        // outColor = vec4((camera * vec4(posWorld.xyz, 1.0)).xz, shadow, 1);
        // outColor = vec4(shadow, (camera * vec4(posWorld.xyz, 1.0)).z, 1334, 2);
//        if (shadow != 0) 
//        {
//            color *= 0.2;
//        }
//        outColor = vec4(sd(posWorld.xyz), 123, 1);
//        outColor = vec4(vec3(sd(posWorld.xyz).y), 1);;
//        outColor = vec4((camera * vec4(posWorld.xyz, 1.0)).xyz, 1);

        //        outColor = vec4(lighting, 1);
//        outColor = vec4(lighting, 1);
        //utColor = vec4(smoothstep(vec3(1, 0, 0), vec3(0, 0, 1), vec3(float(ll) / 5)), 1);
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