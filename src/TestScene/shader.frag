#version 430 core

in VS_OUT
{
	vec3 normal;
} i;

uniform vec3 lookDir;

void main()
{
	vec3 lightDir = vec3(0, -1, -0);
	vec3 color = vec3(1, 1, 0);
	float specular = clamp(1, 0, pow(dot(lookDir, reflect(lightDir, i.normal)), 23));
	gl_FragColor = vec4(clamp(1, 0, dot(lightDir, i.normal)) * color + color * 0.5 + specular * color, 1);	
}

