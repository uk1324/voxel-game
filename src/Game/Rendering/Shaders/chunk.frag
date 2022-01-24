#version 430 core

uniform sampler2DArray blockTextureArray;
uniform sampler2D shadowMap;

in vec2 texCoord;
in flat uint texIndex;
in vec4 FragPosLightSpace;
//in vec3 normal;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = (currentDepth - 0.005) > closestDepth  ? 1.0 : 0.0;

    return shadow;
}  

void main()
{
	vec3 dirLight = normalize(vec3(0, 1, 1));

	//gl_FragColor = texture(sprite, texCoord);
	vec4 tex = texture(blockTextureArray, vec3(texCoord, texIndex));

	//vec4 o = tex * clamp((1 - gl_FragCoord.z) * 50, 0, 1);
	vec4 o = tex;
	if (o.a == 0)
		discard;
	o.a = 1;
	//gl_FragColor = tex;
	// gl_FragColor = tex;
	//gl_FragColor = tex * 0.2 + tex * clamp(dot(dirLight, normal), 0, 1);
	float shadow = ShadowCalculation(FragPosLightSpace);       
	gl_FragColor =  (0.2 + (1.0 - shadow) ) * tex;
}