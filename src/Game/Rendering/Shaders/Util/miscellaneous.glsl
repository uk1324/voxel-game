#define INF uintBitsToFloat(0x7F800000u)

vec3 worldPosFromDepth(vec2 screenTexturePos, float depth, mat4 viewProjectionInverse)
{
	if (depth > 0.9999)
		return vec3(INF);

	vec4 posNdc = vec4(
		screenTexturePos * 2.0 - 1.0 /* screen texture position to NDC(<0, 1> to <-1, 1>) */, 
		depth * 2.0 - 1.0, /* depth from <0, 1> to <-1, 1> */
		1.0);
	vec4 posWorld = viewProjectionInverse * posNdc;
	return posWorld.xyz / posWorld.w;
}

vec3 viewPosFromDepth(vec2 screenTexturePos, float depth, mat4 projectionInverse)
{
	return worldPosFromDepth(screenTexturePos, depth, projectionInverse);
}

vec4 blend(vec4 src, vec4 dest)
{
	return vec4(src.a * src.rgb + (1 - src.a) * dest.rgb, 1);
}