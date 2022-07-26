#version 430 core

layout (location = 0) in uint vert;

const vec3 normals[] = {
 	vec3(-1, 0, 0), // Right
 	vec3(1, 0, 0), // Left
 	vec3(0, 1, 0), // Top
 	vec3(0, -1, 0), // Bottom
 	vec3(0, 0, 1), // Back
 	vec3(0, 0, -1), // Front
 	normalize(vec3(-1, 0, 1)), 
	normalize(vec3(1, 0, 1)), 
 	-normalize(vec3(-1, 0, 1)),
 	-normalize(vec3(1, 0, 1)), 
 };

vec2 texCoords[] = {
	vec2(0, 0),
	vec2(1, 0),
	vec2(0, 1),
	vec2(1, 1)
};

#define X_OFFSET 0
#define Y_OFFSET 4
#define Z_OFFSET Y_OFFSET + 7
#define TEXTURE_INDEX_OFFSET Z_OFFSET + 4
#define TEXTURE_POS_INDEX_OFFSET TEXTURE_INDEX_OFFSET + 10
#define NORMAL_INDEX_OFFSET TEXTURE_POS_INDEX_OFFSET + 2

#define X_MASK ~((~0) << (X_OFFSET + 4))
#define Y_MASK ~((~0) << (Y_OFFSET + 7))
#define Z_MASK ~((~0) << (Z_OFFSET + 4))
#define TEXTURE_INDEX_MASK ~((~0) << (TEXTURE_INDEX_OFFSET + 10))
#define TEXTURE_POS_INDEX_MASK ~((~0) << (TEXTURE_POS_INDEX_OFFSET + 2))
// #define NORMAL_INDEX_MASK ~((~0) << (NORMAL_INDEX_OFFSET + 4))
#define NORMAL_INDEX_MASK ~0

// Rename to world to model
// world to clip
// world to view.
uniform mat4 model;
uniform mat4 projection;
uniform mat4 camera;

out flat uint textureIndex;
out vec2 texturePos;
out vec3 fragNormal;
out vec3 fragPosWorld;


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

uniform float time;


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


void main()
{
	vec3 vertPos = vec3(
		(vert & X_MASK) >> X_OFFSET,
		(vert & Y_MASK) >> Y_OFFSET,
		(vert & Z_MASK) >> Z_OFFSET
	);
	uint normalIndex = (vert & NORMAL_INDEX_MASK) >> NORMAL_INDEX_OFFSET;

	textureIndex = (vert & TEXTURE_INDEX_MASK) >> TEXTURE_INDEX_OFFSET;
	texturePos = texCoords[(vert & TEXTURE_POS_INDEX_MASK) >> TEXTURE_POS_INDEX_OFFSET];	
	// This doesn't change the normal's rotation based on the model matrix.
	// It might be better to just pass the chunks position then it won't be necessary.
	fragNormal = normals[normalIndex];
	fragPosWorld = (model * vec4(vertPos, 1)).xyz;

	fragPosWorld.y -= 0.2;
	float offset = (noise((vec2(fragPosWorld.x, fragPosWorld.z) + vec2(time)) * 0.5) + 1) / 2 * 0.2;
    fragPosWorld.y += offset;
    // float offset = ((fbm(time + vertPos.x, 6, 0.1) * fbm(time + vertPos.z, 6, 0.1) + 1) / 2) * 0.2;

	gl_Position = projection * camera * vec4(fragPosWorld, 1);
}