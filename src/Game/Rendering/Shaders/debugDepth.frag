#version 430 core

uniform sampler2D textureSampler;

in vec2 textureCoord;

out vec4 fragColor;

float linearize_depth(float d,float zNear,float zFar)
{
    float z_n = 2.0 * d - 1.0;
    return 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
}

void main()
{
    float near = 0.1;
    float far = 10.0;
    float depth = texture(textureSampler, textureCoord).r;
    float ndc = depth * 2.0 - 1.0; 
    float linearDepth = (2.0 * near * far) / (far + near - ndc * (far - near));	
	fragColor = vec4(vec3(depth), 1.0);
}