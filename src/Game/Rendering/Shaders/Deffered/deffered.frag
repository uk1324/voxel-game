#version 430 core

vec3 directionalLightDir = normalize(vec3(-0.5, -1, -0.5));

uniform sampler2D shadowMaps[16];
uniform mat4 worldToShadowMap[16];
uniform float cascadeZ[16];
uniform int cascadeCount;
uniform mat4 worldToView;
uniform float farPlane;

float shadow(vec3 posWorldSpace)
{
    vec4 fragPosViewSpace = worldToView * vec4(posWorldSpace, 1.0);
    // This value should be negated or not based on the sign of the forward vector.
    // Just using abs makes it simpler becuase it works with any forward direction.
    float distanceFromView = abs(fragPosViewSpace.z);

    int layer = -1;
    for (int i = 0; i < cascadeCount; ++i)
    {
        if (distanceFromView < cascadeZ[i])
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
    {
        layer = cascadeCount;
    }

    // Perspective divide isn't needed becuase orthographic projection is used.
    vec3 posLightSpace = (worldToShadowMap[layer] * vec4(posWorldSpace, 1.0)).xyz;
    // Transform from NDC to texture range. <[-1, -1], [1, 1]> to <[0, 0], [1, 1]>.
    posLightSpace = posLightSpace * 0.5 + 0.5;

    float currentDepth = posLightSpace.z;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (currentDepth > 1.0)
    {
        return 0.0;
    }

    float bias = 0.00005;
    const float biasModifier = 0.5;
    // Because of precision issues shapes can shadow cast shadows on themselves. This effect is called shadow acne.
    // It can be mitigated by applying a bias to move the value closer than it actually is.
    // If the bias is too big an effect called petter panning happens which makes parts of the shadows disappear.
    // With this constant value this can be visible when looking close up on the edges of objects with shadows on them.
    // The most common techniques add a depth bias calculated based on slope or add the normal to the position of the point.
    // I couldn't get good result with neither of these methods. I decided to just use a constant bias varied based on the 
    // cascade though this way creates visible transitions between cascaded(further cascaded have more acne).
    // Should also probably scale the bias based on the shadow map resolution.
    if (layer == cascadeCount)
    {
        bias *= 1 / (farPlane * biasModifier);
    }
    else
    {
        bias *= 1 / (cascadeZ[layer] * biasModifier);
    }

    float closestDepth = texture2D(shadowMaps[layer], posLightSpace.xy).r;

    // Average texels around the point. This is also called percentage-closer filtering.
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMaps[layer], 0));
    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            float closestDepth = texture2D(shadowMaps[layer], posLightSpace.xy + vec2(x, y) * texelSize).r;
            shadow += (currentDepth - bias) > closestDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    return shadow;
}

uniform sampler2D geometryPosWorld;
uniform sampler2D geometryAlbedo;
uniform sampler2D geometryNormal;
uniform sampler2D geometryDepth;

in vec2 texturePos;

out vec4 outColor;






#define HASHSCALE 0.1031

float hash(float p)
{
	vec3 p3  = fract(vec3(p) * HASHSCALE);
    p3 += dot(p3, p3.yzx + 19.19);
    return fract((p3.x + p3.y) * p3.z);
}

float fade(float t) { return t*t*t*(t*(6.*t-15.)+10.); }

float grad(float hash, float p)
{
    int i = int(1e4*hash);
	return (i & 1) == 0 ? p : -p;
}

float perlinNoise1D(float p)
{
	float pi = floor(p), pf = p - pi, w = fade(pf);
    return mix(grad(hash(pi), pf), grad(hash(pi + 1.0), pf - 1.0), w) * 2.0;
}

float fbm(float pos, int octaves, float persistence) 
{
    float total = 0., frequency = 1., amplitude = 1., maxValue = 0.;
    for(int i = 0; i < octaves; ++i) 
    {
        total += perlinNoise1D(pos * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= 2.;
    }
    return total / maxValue;
}

#define M_PI 3.14159265358979323846

float rand(vec2 co){return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);}
float rand (vec2 co, float l) {return rand(vec2(rand(co), l));}
float rand (vec2 co, float l, float t) {return rand(vec2(rand(co, l), t));}

float perlin(vec2 p, float dim, float time) {
	vec2 pos = floor(p * dim);
	vec2 posx = pos + vec2(1.0, 0.0);
	vec2 posy = pos + vec2(0.0, 1.0);
	vec2 posxy = pos + vec2(1.0);
	
	float c = rand(pos, dim, time);
	float cx = rand(posx, dim, time);
	float cy = rand(posy, dim, time);
	float cxy = rand(posxy, dim, time);
	
	vec2 d = fract(p * dim);
	d = -0.5 * cos(d * M_PI) + 0.5;
	
	float ccx = mix(c, cx, d.x);
	float cycxy = mix(cy, cxy, d.x);
	float center = mix(ccx, cycxy, d.y);
	
	return center * 2.0 - 1.0;
}

// p must be normalized!
float perlin(vec2 p, float dim) {
	
	vec2 pos = floor(p * dim);
	vec2 posx = pos + vec2(1.0, 0.0);
	vec2 posy = pos + vec2(0.0, 1.0);
	vec2 posxy = pos + vec2(1.0);
	
	//// For exclusively black/white noise
	//float c = step(rand(pos, dim), 0.5);
	//float cx = step(rand(posx, dim), 0.5);
	//float cy = step(rand(posy, dim), 0.5);
	//float cxy = step(rand(posxy, dim), 0.5);
	
	float c = rand(pos, dim);
	float cx = rand(posx, dim);
	float cy = rand(posy, dim);
	float cxy = rand(posxy, dim);
	
	vec2 d = fract(p * dim);
	d = -0.5 * cos(d * M_PI) + 0.5;
	
	float ccx = mix(c, cx, d.x);
	float cycxy = mix(cy, cxy, d.x);
	float center = mix(ccx, cycxy, d.y);
	
	return center * 2.0 - 1.0;
	return perlin(p, dim, 0.0);
}

vec2 hash( vec2 p ) // replace this by something better
{
	p = vec2( dot(p,vec2(127.1,311.7)), dot(p,vec2(269.5,183.3)) );
	return -1.0 + 2.0*fract(sin(p)*43758.5453123);
}

float noise( in vec2 p )
{
    const float K1 = 0.366025404; // (sqrt(3)-1)/2;
    const float K2 = 0.211324865; // (3-sqrt(3))/6;

	vec2  i = floor( p + (p.x+p.y)*K1 );
    vec2  a = p - i + (i.x+i.y)*K2;
    float m = step(a.y,a.x); 
    vec2  o = vec2(m,1.0-m);
    vec2  b = a - o + K2;
	vec2  c = a - 1.0 + 2.0*K2;
    vec3  h = max( 0.5-vec3(dot(a,a), dot(b,b), dot(c,c) ), 0.0 );
	vec3  n = h*h*h*h*vec3( dot(a,hash(i+0.0)), dot(b,hash(i+o)), dot(c,hash(i+1.0)));
    return dot( n, vec3(70.0) );
}

float norm(float n) 
{
	return (n + 1) / 2;
}


uniform float time;


#define PI 3.1415926538
void main() 
{
    vec3 color = texture2D(geometryAlbedo, texturePos).rgb;
    vec3 posWorld = texture2D(geometryPosWorld, texturePos).xyz;
    vec3 normal = normalize(texture(geometryNormal, texturePos).xyz); 
    float depth = texture2D(geometryDepth, texturePos).r;

    float shadow = shadow(posWorld);
    float ambient = 0.3;
    float diffuse = clamp(dot(normal, -directionalLightDir), 0, 1);
    float specular = clamp(pow(dot(reflect(-directionalLightDir, normal), normal), 1), 0, 1);
    color *= ambient + ((1 - shadow) * diffuse);
    outColor = vec4(color, 1);
    gl_FragDepth = depth;






//    vec3 pos = posWorld + vec3(time * 0.5);
//    vec2 p2 = vec2(pos.x, pos.z);
//    vec2 p = vec2(pos.x, pos.z);
//    vec2 n = normalize(vec2(-1, -1));
//    vec2 n2 = normalize(vec2(1, -1));
//    float c = norm(perlinNoise1D(dot(p + vec2(norm(fbm(dot(p2, n2) * 0.2, 6, 0.5))) * (clamp(norm(perlinNoise1D(time * 0.3)) + 0.2, 0.0, 1.2)), n)));
//	//outColor = vec4(vec3(axis) * vec3(0, 0, 0.7), 0.6);
//	// outColor = vec4(vec3(axis) * vec3(0, 0.8, 1), 0.8);
//	outColor = vec4(vec3(c) * vec3(1, 1, 1), 1);
}