#version 430 core

uniform sampler2D textureSampler;

in VsOut
{
	vec3 normal;
	vec2 uv;
} i;

out vec4 outColor;

void main()
{
	//vec3 lightDir = normalize(vec3(1, -1, 0));
	//outColor = (dot(lightDir, i.normal) + 0.2) * vec4(1, 0, 0, 1);
	//if (texture(textureSampler, i.uv).r == 12312) discard;
	outColor = texture(textureSampler, i.uv);
	//outColor = vec4(i.uv.x, i.uv.y, 0, 1);
	//outColor = vec4(0, 1, 0, 1);
}