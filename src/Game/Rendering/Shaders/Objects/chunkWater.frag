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

float waterNoise(vec3 fragPosWorld)
{
    vec2 pos = fragPosWorld.xz + (time * 0.5);
    vec2 lineNormal1 = normalize(vec2(-1, -1));
    vec2 lineNormal2 = normalize(vec2(1, -1));
	float signedDistanceFromLine2 = dot(pos, lineNormal2);
	vec2 waveOffset = vec2(norm(fbm(signedDistanceFromLine2 * 0.2, 6, 0.5)));
	float waveOffsetScale = clamp(norm(perlinNoise1D(time * 0.3)) + 0.2, 0.0, 1.2);
    return norm(perlinNoise1D(dot(pos + waveOffset * waveOffsetScale, lineNormal1)));
}

void main()
{
	vec3 directionalLightDir = normalize(vec3(-0.5, -1, -0.5));

	vec3 pos1 = fragPosWorld;
	vec3 pos2 = fragPosWorld + vec3(2.5, 0, 2.5);
	float n1 = waterNoise(pos1);
	float n2 = waterNoise(pos2);
	pos1.y += n1;
	pos2.y += n2;

	vec3 line = normalize(pos1 - pos2);

	vec3 nor = normalize(fragNormal - dot(line, fragNormal) * line);

	vec3 waterColor = vec3(0, 0.7, 1);
	float waterAlpha = 0.8;

	outColor = vec4(n1 * vec3(0, 0.7, 1), 0.8);

	vec3 viewDir = normalize(viewPos - fragPosWorld);
	vec3 reflectionDir = reflect(directionalLightDir, nor);
    float specular = clamp(pow(dot(reflectionDir, viewDir), 10), 0, 1);

	// 1D noise going in the direction perpendicular to the waves.
	float na = norm(perlinNoise1D(dot(normalize(vec2(-1, 1)), fragPosWorld.xz) + time * 2));

	outColor = vec4(0.2 * waterColor * (norm(n1) + 0.2 + na * 0.5) + vec3(0.7) * specular, waterAlpha);
//	outColor = vec4(0.6 * vec3(1) * (norm(n1) + 0.2 + na * 0.5), 1);
	//outColor = vec4(vec3(norm(perlinNoise1D(dot(normalize(vec2(1, -1)), fragPosWorld.xz)))), 1);
}