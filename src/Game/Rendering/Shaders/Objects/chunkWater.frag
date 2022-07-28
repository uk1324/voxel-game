#version 430 core

uniform sampler2DArray blockTextureArray;
uniform vec3 viewPos;
uniform float time;

in flat uint textureIndex;
in vec2 texturePos;
in vec3 fragNormal;
in vec3 fragPosWorld;

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

uniform int octaves;
uniform float persistence;
uniform float diagonalNormalOffset;
uniform float inputNoiseScale;
uniform int specularIntensity;
uniform float diagonalScale;
uniform float diagonalNoiseScale;
uniform vec3 colorWater;
uniform vec3 colorSpecular;
uniform float timeScale;

uniform sampler2D background;

float on(vec2 pos)
{
	float accumulator = 0, scale = 1;
	for (int i = 0; i < octaves; i++)
	{
		accumulator += noise(pos * scale);
		scale *= persistence;
	}
	return accumulator;
}

vec4 blend(vec4 src, vec4 dest)
{
	return vec4(src.a * src.rgb + (1 - src.a) * dest.rgb, 1);
}

uniform vec2 screenSize;

void main()
{
	vec3 directionalLightDir = normalize(vec3(-0.5, -1, -0.5));

	float offset = dot(fragPosWorld.xz, normalize(vec2(1, -1)));
	vec3 v1 = fragPosWorld;
	vec3 v2 = v1 + vec3(diagonalNormalOffset, 0, 0); 
	vec3 v3 = v1 + vec3(0, 0, diagonalNormalOffset);
	float diagonalDistance = dot(normalize(vec2(1, 1)), v1.xz);
	float noiseScale = perlinNoise1D(diagonalDistance * diagonalScale + time * timeScale) * diagonalNoiseScale;
	// Try with more octaves
	v1.y += on(v1.xz * inputNoiseScale) * noiseScale;
	v2.y += on(v2.xz * inputNoiseScale) * noiseScale;
	v3.y += on(v3.xz * inputNoiseScale) * noiseScale;
	vec3 normal = normalize(cross(v2 - v1, v3 - v1));

	vec3 viewDir = normalize(viewPos - fragPosWorld);
	vec3 reflectionDir = reflect(directionalLightDir, normal);
    float specular = clamp(pow(dot(reflectionDir, viewDir), specularIntensity) + 0.2, 0, 1);
	specular = (specular + 1) / 2;
	//float specular = clamp(pow(dot(reflect(-directionalLightDir, normal), normal), specularIntensity), 0, 1);

	// outColor = vec4(vec3(v1.y - fragPosWorld.y), 1);
//	outColor = vec4(vec3(colorWater) + colorSpecular * specular / 2, 0.6);
	vec4 color = texture(background, gl_FragCoord.xy / screenSize + noiseScale * 0.01);
//	vec4 color = texture(background, gl_FragCoord.xy / screenSize);
	outColor = blend(vec4(vec3(colorWater) + colorSpecular * specular / 2, 0.6), color);
	//vec2 pos = gl_FragCoord.xy / screenSize + noiseScale * 0.02;

	//outColor = texture(background, pos);
	
	// outColor = vec4((gl_FragCoord.xy / screenSize + noiseScale * 0.02), 0, 1);
	//outColor = vec4((gl_FragCoord.xy / 2 + 0.5) / vec2(800, 600), 0, 1);
	//outColor = color;
	//outColor = vec4(vec3(colorWater * (0.3 + specular)), 0.6);
}