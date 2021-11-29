#version 430 core

uniform sampler2DArray blockTextureArray;

in vec2 texCoord;
in flat uint texIndex;
//in vec3 normal;

void main()
{
	vec3 dirLight = normalize(vec3(0, 1, 1));

	//gl_FragColor = texture(sprite, texCoord);
	vec4 tex = texture(blockTextureArray, vec3(texCoord, texIndex));
	gl_FragColor = tex;
	//gl_FragColor = tex * 0.2 + tex * clamp(dot(dirLight, normal), 0, 1);
}