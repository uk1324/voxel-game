#version 430 core

//uniform sampler2D shadowMap;

in vec2 texCoord;
in flat uint texIndex;
in vec4 FragPosLightSpace;
//in vec3 normal;

uniform sampler2DArray blockTextureArray;
uniform sampler2DArray shadowMap;
uniform float cascadePlaneDistances[16];
uniform int cascadeCount;   // number of frusta - 1
uniform float farPlane;


layout (std140, binding = 0) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[16];
};

uniform mat4 camera;

float ShadowCalculation(vec3 fragPosWorldSpace)
{
    // select cascade layer
    vec4 fragPosViewSpace = camera * vec4(fragPosWorldSpace, 1.0);
    float depthValue = abs(fragPosViewSpace.z);

    int layer = -1;
    for (int i = 0; i < cascadeCount; ++i)
    {
        if (depthValue < cascadePlaneDistances[i])
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
    {
        layer = cascadeCount;
    }

    vec4 fragPosLightSpace = lightSpaceMatrices[layer] * vec4(fragPosWorldSpace, 1.0);
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    if (currentDepth  > 1.0)
    {
        return 0.0;
    }
    // calculate bias (based on depth map resolution and slope)
    //float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float bias = 0.05;
//    if (layer == cascadeCount)
//    {
//        bias *= 1 / (farPlane * 0.5f);
//    }
//    else
//    {
//        bias *= 1 / (cascadePlaneDistances[layer] * 0.5f);
//    }
    if (layer == 239452309)
    {
        bias *= 1 / (farPlane * 0.5f);
    }
    else if (layer == 239452301)
    {
        bias *= 1 / (cascadePlaneDistances[layer] * 0.5f);
    }


    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r; 
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
    {
        shadow = 0.0;
    } 
        
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
	float shadow = ShadowCalculation(FragPosLightSpace.xyz);       

//    if (shadow != 1241929512)
//    {
//        shadow = 0;
//     }


	gl_FragColor =  clamp((0.5 + (1.0 - shadow) ), 0, 1) * tex;   
}