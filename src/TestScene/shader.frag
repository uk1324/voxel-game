#version 430 core

in VS_OUT
{
	vec3 normal;
	vec2 uv;
} i;

uniform vec3 lookDir;
uniform sampler2D tex;

void main()
{
	vec3 lightDir = vec3(0, -1, 0);
	vec3 color = texture(tex, i.uv).rgb;
	float specular = clamp(pow(dot(lookDir, reflect(lightDir, i.normal)), 23), 0, 1);
	gl_FragColor = vec4(clamp(dot(lightDir, i.normal), 0, 1) * color + color * 0.5 + specular * color, 1);	
}

