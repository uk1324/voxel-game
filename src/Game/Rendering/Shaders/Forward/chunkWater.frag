#version 430 core

#include "../Util/noise.glsl"

uniform vec3 viewPos;
uniform float time;
uniform vec2 screenSize;

uniform int octaves;
uniform float persistence;
uniform float diagonalNormalOffset;
uniform float inputNoiseScale;
uniform int specularIntensity;
uniform float diagonalScale;
uniform float diagonalNoiseScale;
uniform vec3 colorWater;
uniform vec3 colorSpecular;
uniform float timeScale;

uniform sampler2D background;

in flat uint textureIndex;
in vec2 texturePos;
in vec3 fragNormal;
in vec3 fragPosWorld;

out vec4 outColor;

vec4 blend(vec4 src, vec4 dest)
{
	return vec4(src.a * src.rgb + (1 - src.a) * dest.rgb, 1);
}

float waveHeightMapNoise(vec2 pos)
{
	float accumulator = 0, scale = 1;
	for (int i = 0; i < octaves; i++)
	{
		accumulator += perlin(pos * scale);
		scale *= persistence;
	}
	return accumulator;
}








uniform sampler2D shadowMaps[16];
uniform mat4 worldToShadowMap[16];
uniform float cascadeZ[16];
uniform int cascadeCount;
uniform mat4 worldToView;
uniform float farPlane;



float shadow(vec3 posWorldSpace)
{
    vec4 fragPosViewSpace = worldToView * vec4(posWorldSpace, 1.0);
    // This value should be negated or not based on the sign of the forward vector.
    // Just using abs makes it simpler becuase it works with any forward direction.
    float distanceFromView = abs(fragPosViewSpace.z);

    int layer = -1;
    for (int i = 0; i < cascadeCount; ++i)
    {
        if (distanceFromView < cascadeZ[i])
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
    {
        layer = cascadeCount;
    }

    // Perspective divide isn't needed becuase orthographic projection is used.
    vec3 posLightSpace = (worldToShadowMap[layer] * vec4(posWorldSpace, 1.0)).xyz;
    // Transform from NDC to texture range. <[-1, -1], [1, 1]> to <[0, 0], [1, 1]>.
    posLightSpace = posLightSpace * 0.5 + 0.5;

    float currentDepth = posLightSpace.z;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (currentDepth > 1.0)
    {
        return 0.0;
    }

    float bias = 0.00005;
    const float biasModifier = 0.5;
    // Because of precision issues shapes can shadow cast shadows on themselves. This effect is called shadow acne.
    // It can be mitigated by applying a bias to move the value closer than it actually is.
    // If the bias is too big an effect called petter panning happens which makes parts of the shadows disappear.
    // With this constant value this can be visible when looking close up on the edges of objects with shadows on them.
    // The most common techniques add a depth bias calculated based on slope or add the normal to the position of the point.
    // I couldn't get good result with neither of these methods. I decided to just use a constant bias varied based on the 
    // cascade though this way creates visible transitions between cascaded(further cascaded have more acne).
    // Should also probably scale the bias based on the shadow map resolution.
    if (layer == cascadeCount)
    {
        bias *= 1 / (farPlane * biasModifier);
    }
    else
    {
        bias *= 1 / (cascadeZ[layer] * biasModifier);
    }

    float closestDepth = texture2D(shadowMaps[layer], posLightSpace.xy).r;

    // Average texels around the point. This is also called percentage-closer filtering.
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMaps[layer], 0));
    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            float closestDepth = texture2D(shadowMaps[layer], posLightSpace.xy + vec2(x, y) * texelSize).r;
            shadow += (currentDepth - bias) > closestDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    return shadow;
}



void main()
{
	vec3 directionalLightDir = normalize(vec3(-0.5, -1, -0.5));

	vec3 v1 = fragPosWorld;
	vec3 v2 = v1 + vec3(diagonalNormalOffset, 0, 0); 
	vec3 v3 = v1 + vec3(0, 0, diagonalNormalOffset);
	float distancePerpendicularToWaves = dot(normalize(vec2(1, 1)), v1.xz);
	float noiseScale = perlin(distancePerpendicularToWaves * diagonalScale + time * timeScale) * diagonalNoiseScale;
	v1.y += waveHeightMapNoise(v1.xz * inputNoiseScale) * noiseScale;
	v2.y += waveHeightMapNoise(v2.xz * inputNoiseScale) * noiseScale;
	v3.y += waveHeightMapNoise(v3.xz * inputNoiseScale) * noiseScale;
	vec3 normal = normalize(cross(v2 - v1, v3 - v1));

	vec3 viewDir = normalize(viewPos - fragPosWorld);
	vec3 reflectionDir = reflect(directionalLightDir, normal);
	float specular = clamp(pow(dot(reflectionDir, viewDir), specularIntensity), 0, 1);

	vec4 refractedBackgroundColor = texture(background, gl_FragCoord.xy / screenSize + noiseScale * 0.01);

	// Could also multiply colorSpecular and/or waterColor by diffuse.
	// float diffuse = clamp(dot(-directionalLightDir, normal or fragNormal), 0, 1);
	outColor = blend(vec4(vec3(colorWater) + colorSpecular * specular / 2, 0.8), refractedBackgroundColor);
}