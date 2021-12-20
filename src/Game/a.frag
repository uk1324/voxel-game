#version 430 core

out vec4 outColor;

uniform vec3 color;

void main()
{    
    gl_FragColor = vec4(color, 1);
    //outColor = vec4(1, 1, 0, 1);
}