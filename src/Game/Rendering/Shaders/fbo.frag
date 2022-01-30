#version 430 core

uniform sampler2D textureSampler;
uniform float time;

in vec2 texturePos;

out vec4 outColor;

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
	// Upside down
	//outColor = texture(textureSampler, vec2(texturePos.x, 1 - texturePos.y));
	
	// Box blur
	//vec2 size = textureSize(textureSampler, 0);
	//vec2 texelSize = vec2(1.0) / size;
	//vec4 color = vec4(0);
	//float kernelSize = 5.0;
	//for (float x = -kernelSize; x <= kernelSize ; x++)
	//{
	//	for (float y = -kernelSize; y <= kernelSize ; y++)
	//	{
	//		color += texture(textureSampler, texturePos + vec2(texelSize.x * x, texelSize.y * y));
	//	}
	//}
	//outColor = color / ((kernelSize * 2 + 1) * (kernelSize * 2 + 1));

	//outColor = texture(textureSampler, texturePos + texelSize);
	//outColor = vec4(vec3((textureColor.r + textureColor.g + textureColor.b) / 3.0), 1.0);

	//vec4 textureColor = texture(textureSampler, vec2(texturePos.x, texturePos.y));
	//if (int(gl_FragCoord.x / 3) % 2 == int(gl_FragCoord.y / 3) % 2)
	//{
	//	outColor = textureColor * 1.5;
	//}
	//else
	//{
	//	outColor = textureColor;
	//}

//	vec2 size = textureSize(textureSampler, 0);
//	vec2 texelSize = vec2(1.0) / size;
//	vec4 color = vec4(0);
//	float kernelSize = 1.0;
//
//	float array[] = {
//		1, 1, 1,
//		1, -8, 1,
//		1, 1, 1,
//	};
////	float array[] = {
////		-1, -1, -1,
////		-1,  8, -1,
////		-1, -1, -1,
////	};
//	for (float x = -kernelSize; x <= kernelSize ; x++)
//	{
//		for (float y = -kernelSize; y <= kernelSize ; y++)
//		{
//			color += texture(textureSampler, texturePos + vec2(texelSize.x * x, texelSize.y * y)) * array[int((y + 1) * 3 + (x + 1))];
//		}
//	}
//	vec4 textureColor = texture(textureSampler, texturePos);
//	outColor = color;
	////outColor =  textureColor + color;

	// Median blur - really slow
	//vec2 size = textureSize(textureSampler, 0);
	//vec2 texelSize = vec2(1.0) / size;
	//const float kernelSize = 2.0;
	//vec4 values[9];
	//int valuesSize = 0;
	//for (float x = -kernelSize; x <= kernelSize; x++)
	//{
	//	for (float y = -kernelSize; y <= kernelSize; y++)
	//	{
	//		vec4 color = texture(textureSampler, texturePos + vec2(texelSize.x * x, texelSize.y * y));
	//		if (valuesSize == 0)
	//		{
	//			values[0] = color;
	//			valuesSize++;
	//		}
	//		else
	//		{
	//			bool found = false;	
	//			for (int i = 0; i < valuesSize; i++)
	//			{
	//				if (dot(color, color) < dot(values[i], values[i]))
	//				{
	//					for (int j = valuesSize - 1; j >= i; j--)
	//					{
	//						values[j + 1] = values[j];
	//					}
	//					values[i] = color;
	//					valuesSize++;
	//					found = true;
	//					break;
	//				}
	//			}
	//			if (found == false)
	//			{
	//				values[valuesSize] = color;
	//				valuesSize++;
	//			}
	//		}
	//	}
	//}
	//outColor = values[4];


	// Smaller collor range
	//vec4 color = texture(textureSampler, texturePos);
	//outColor = round(color * 3) / 3;

	// Wavy
	//float v = cos(texturePos.y * 100 + time);
	//v = (v + 1.0) / 2.0;
	//v /= 100;
	//float v2 = cos(texturePos.x * 100 + time);
	//v2 = (v2 + 1.0) / 2.0;
	//v2 /= 100;
	//vec4 color = texture(textureSampler, texturePos + vec2(v, v2));
	//outColor = color + color * vec4(hsv2rgb(vec3(fract(time / 3), 1, 1)), 1) / 2;

	// Something
	//if (time == 12312)
	//	discard;
	//vec2 a;
	//if (length(texturePos * 2 - 1) > 0.5)
	//{
	//	a = (normalize(texturePos * 2 - 1) + 1) / 2.0;
	//}
	//else
	//{
	//	a = texturePos;
	//}
	//vec4 color = texture(textureSampler, a);
	//outColor = color;

	// Circle thingy
//	if (time == 12312)
//		discard;
//	vec2 size = textureSize(textureSampler, 0);
//	vec2 texelSize = vec2(1.0) / size;
//	vec4 maxColor = vec4(0);
//	vec2 maxColorPos;
//	float kernelSize = 5.0;
//	for (float x = -kernelSize; x <= kernelSize ; x++)
//	{
//		for (float y = -kernelSize; y <= kernelSize ; y++)
//		{
//			vec4 color = texture(textureSampler, texturePos + vec2(texelSize.x * x, texelSize.y * y));
//			if (length(color) > length(maxColor))
//			{
//				maxColor = color;
//				maxColorPos = texturePos + vec2(texelSize.x * x, texelSize.y * y);
//			}
//		}
//	}
//	
//	vec2 pos = texturePos;
//	pos.x = pos.x * size.x / size.y;
//	maxColorPos.x = maxColorPos.x * size.x / size.y;
//	if (distance(pos, maxColorPos) < texelSize.y * 5)
//	{
//		outColor = maxColor;
//	}
// // Comment this
//	else
//	{
//		outColor = texture(textureSampler, texturePos);
//	}

	if (time == 12931923)
		discard;

	outColor = texture(textureSampler, texturePos);

}