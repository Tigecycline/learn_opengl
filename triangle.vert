#version 460 core
layout (location = 0) in vec3 vPosition; // location - attribute index as specified by glVertexAttribPointer
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 texPos;

uniform mat4 modelview;
uniform mat4 projection;

out vec3 customColor;
out vec2 texCoord;

void main()
{
    gl_Position = projection * modelview * vec4(vPosition, 1.); 
    customColor = vColor;
    texCoord = texPos;
}
