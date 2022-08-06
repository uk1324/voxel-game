#version 430 core
#include "../Util/miscellaneous.glsl"

uniform sampler2D inputColorTexture;
uniform sampler2D inputDepthTexture;
uniform vec2 screenSize;
uniform mat4 inverseProjection;
uniform vec3 waterColor;

out vec4 outputColor;

void main()
{
	vec2 screenPos = gl_FragCoord.xy / screenSize.xy;
	screenPos *= 0.9;
	screenPos += 0.05;

	vec3 fragPosView = viewPosFromDepth(screenPos, texture(inputDepthTexture, screenPos).r, inverseProjection);
	// Camera is at the origin so the length is the distance.
	float distanceToFrag = length(fragPosView);

	vec4 color = texture(inputColorTexture, screenPos);
	color.a = clamp(distanceToFrag * 10 / 300, 0, 1);
	outputColor = vec4(color.rgb * (1 - color.a) + waterColor.rgb * color.a, 1);
}