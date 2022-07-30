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