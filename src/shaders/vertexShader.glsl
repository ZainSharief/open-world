#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 aTexCoord;

out vec3 fragColor;
out vec2 TexCoord;

uniform mat4 transform;

void main()
{
    fragColor = color;
    TexCoord = aTexCoord;
    gl_Position = transform * vec4(aPos, 1.0);
}