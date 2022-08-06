#version 430 core
#include "shadowSamples.glsl"

uniform vec3 directionalLightDir;

uniform sampler2D shadowMaps[16];
uniform mat4 worldToShadowMap[16];
uniform float cascadeZ[16];
uniform vec3 cascadeScale[16];
uniform int cascadeCount;
uniform mat4 worldToView;
uniform float farPlane;

// TODO: Interpolating in the arena between the cascades would reduce seams between them.
float shadow(vec3 posWorldSpace, vec3 normal)
{
    vec4 fragPosViewSpace = worldToView * vec4(posWorldSpace, 1.0);
    // This value should be negated or not based on the sign of the forward vector.
    // Just using abs makes it simpler becuase it works with any forward direction.
    float distanceFromView = abs(fragPosViewSpace.z);

    // If not found then use the furthest cascade.
    int layer = cascadeCount;
    for (int i = 0; i < cascadeCount; ++i)
    {
        if (distanceFromView < cascadeZ[i])
        {
            layer = i;
            break;
        }
    }
    float cosNormalLight = dot(normal, directionalLightDir);
    float sinNormalLight = sqrt(1.0 - cosNormalLight * cosNormalLight);
    float normalBiasScale = 0.087;
    posWorldSpace += normal * sinNormalLight * normalBiasScale;

    vec3 posLightSpace = (worldToShadowMap[layer] * vec4(posWorldSpace, 1.0)).xyz;
    posLightSpace = posLightSpace * 0.5 + 0.5;
    //posLightSpace *= vec3(0.5, 0.5, 1.0);
    //posLightSpace += vec3(0.5, 0.5, 0.5);

    float currentDepth = posLightSpace.z;

    if (currentDepth > 1.0)
    {
        return 0.0;
    }

    float constantBias = 8.09717e-5;

    // TODO: This value can get really big if the angle between the normal and the light is near zero. Set some max value for it.
    // TODO: This uses the geometry buffer normals not the surface normals. Either store an additional buffer with the surface normals or apply the effect in the shadow fragment shader.
    float tanNormalLight = sinNormalLight / cosNormalLight;
    float slopeBiasScale = -0.000161943;
    float slopeBias = tanNormalLight * slopeBiasScale;

    vec2 texelSize = 1.0 / vec2(textureSize(shadowMaps[layer], 0));
    float shadow = 0.0;
    for (int i = 0; i < shadowSamplesPositions.length(); i++)
    {
        vec2 sampleOffset = shadowSamplesPositions[i] * cascadeScale[layer].x * 20  * texelSize;
        float closestDepth = texture2D(shadowMaps[layer], posLightSpace.xy + sampleOffset).r;
        // Some articles suggest to scale the bias by the size of the cascade, but I couldn't get that to work well.
        shadow += (currentDepth - constantBias - slopeBias) > closestDepth ? 1.0 : 0.0;
    } 
    return shadow / shadowSamplesPositions.length();
}

uniform sampler2D geometryPosWorld;
uniform sampler2D geometryAlbedo;
uniform sampler2D geometryNormal;
uniform sampler2D geometryDepth;

in vec2 texturePos;

out vec4 outColor;

uniform mat4 viewProjection;

float transformDepth(float depth)
{
    float farPlaneZ = 300.0;
    float nearPlaneZ = 0.1;
    // Fron NDC to [0, 1].
    depth = 2.0 * depth - 1.0;
    return (2.0 * nearPlaneZ * farPlaneZ / (farPlaneZ + nearPlaneZ - depth * (farPlaneZ - nearPlaneZ))) / farPlaneZ;
}

float transformDepth2(float depth)
{
    float farPlaneZ = 300.0;
    float nearPlaneZ = 0.1;
    // Fron NDC to [0, 1].
    return (2.0 * nearPlaneZ * farPlaneZ / (farPlaneZ + nearPlaneZ - depth * (farPlaneZ - nearPlaneZ))) / farPlaneZ;
}

void main() 
{
    vec3 color = texture2D(geometryAlbedo, texturePos).rgb;
    vec3 posWorld = texture2D(geometryPosWorld, texturePos).xyz;
    vec3 normal = normalize(texture(geometryNormal, texturePos).xyz); 
    float depth = texture2D(geometryDepth, texturePos).r;

    float shadow = shadow(posWorld, normal);
    float ambient = 0.3;
    float diffuse = clamp(dot(normal, -directionalLightDir), 0, 1);
    float specular = clamp(pow(dot(reflect(-directionalLightDir, normal), normal), 1), 0, 1);
    color *= ambient + ((1 - shadow) * diffuse);
    outColor = vec4(color, 1);
    gl_FragDepth = depth;

    vec3 samples[] = {
        vec3(0.5358694976759423, 0.7249107820774857, -0.06921258686469448), vec3(0.29920130278169216, 0.722981360626327, -0.23076620131506728), vec3(-0.31170224911987976, 0.43693660112387234, -0.30865640639922265), vec3(-0.061903374913866105, -0.08974832750851826, -0.18207249823606034), vec3(0.26670838056628265, -0.69650922315103, 0.40935022350321154), vec3(0.5460278714461668, 0.2514210119520218, -0.7340974460411539), vec3(0.03691217150080336, -0.4293564630328476, 0.8294033860141709), vec3(0.0038311853305861418, 0.001008196054129781, -0.08199256695864836), vec3(-0.5018294352762095, 0.17293162986318975, 0.38779842642890017), vec3(-0.12068408722137246, 0.04533483202480033, -0.5882460365281857), vec3(0.04022079892381855, -0.03118526242574131, -0.19678362962934695), vec3(0.12476411477716888, -0.03420067406015631, 0.1626859812866242), vec3(0.3210678426937362, 0.315678841943967, -0.1375989420312205), vec3(0.1611890205936811, 0.10784594364181255, -0.5512599757159314), vec3(0.10200698196193779, 0.31949847462158604, 0.6399980907529851), vec3(-0.058820623708951025, -0.3624264235628833, 0.9184767963280693), vec3(0.017642020486803613, -0.07846928457932552, 0.47245613103145423), vec3(0.09478659061823226, 0.26799064241273807, 0.0994130907411361), vec3(0.6069789243935922, 0.5575047464521502, -0.1333658790602329), vec3(0.26505883007819975, -0.008404881263144414, -0.6189701868911146), vec3(-0.5730018717138219, 0.40614950519187404, -0.14146867817652015)
    };

    float ao = 0.0;
    for (int i = 0; i < samples.length(); i++)
    {        
        vec3 samplePosWorld = posWorld + samples[i];
        vec4 samplePosNdc = viewProjection * vec4(samplePosWorld, 1.0);
        vec3 samplePos = samplePosNdc.xyz / samplePosNdc.w;
        float sampleDepth = transformDepth2(samplePos.z);
        if (sampleDepth > depth)
        {
            ao += 1.0;
        }
    }

    //vec3 samplePosWorld = posWorld;
    //vec4 samplePosNdc = viewProjection * vec4(samplePosWorld, 1.0);
    //float sampleDepth = samplePosNdc.z / samplePosNdc.w;
    //outColor = vec4(vec3(ao / samples.length()), 1.0);
    //outColor = vec4(vec3(transformDepth2(sampleDepth)), 1.0);
    //if (gl_FragCoord.x < 1000)
    //{
    //    outColor = vec4(vec3(transformDepth(depth)), 1.0);
    //}
}