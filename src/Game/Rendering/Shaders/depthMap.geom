#version 430 core

layout(triangles, invocations = 5) in;
layout(triangle_strip, max_vertices = 3) out;

in flat uint texIndex[];
in vec2 texCoord[];
out flat uint texIndexG;
out vec2 texCoordG;

layout (std140, binding = 0) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[16];
};

void main()
{          
	for (int i = 0; i < 3; ++i)
	{
		gl_Position = lightSpaceMatrices[gl_InvocationID] * gl_in[i].gl_Position;
		gl_Layer = gl_InvocationID;
		texIndexG = texIndex[i];
		texCoordG = texCoord[i];
		EmitVertex();
	}
	EndPrimitive();
}  