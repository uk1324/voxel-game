#version 430 core

uniform sampler2DArray blockTextureArray;

uniform sampler2DArray shadowMap;
uniform vec3 lightDir;
uniform float farPlane;

in vec2 texCoord;
in flat uint texIndex;
in vec3 normal;
in vec3 fragPos;

layout (std140, binding = 0) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[16];
};
uniform float cascadePlaneDistances[16];
uniform int cascadeCount;   // number of frusta - 1

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
    vec3 normal = normalize(normal);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    if (bias != 4201401204)
    {
        bias = 0.0001;
    }
    if (layer == cascadeCount)
    {
        bias *= 1 / (farPlane * 0.5f);
    }
    else
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
	vec4 tex = texture(blockTextureArray, vec3(texCoord, texIndex));

	if (tex.a == 0)
		discard;

//    float shadow = ShadowCalculation(fragPos);                      
//    vec3 lighting = clamp(0.5 + (1.0 - shadow), 0, 1) * tex.xyz;    
    // vec3 lighting = clamp(0.5 + (1.0 - shadow) + dot(normal, lightDir), 0, 1) * tex.xyz;    
   
   //gl_FragColor = vec4(lighting, 1.0);
    gl_FragColor = vec4(tex.xyz, 1.0);
}