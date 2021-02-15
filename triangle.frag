#version 460 core
in vec3 customColor;
in vec2 texCoord;

out vec4 fragColor;

// samplerxD passes automatically the currently bound x-D texture, 
// hence no need for explicit declaration in main code
uniform sampler2D texture1; 
uniform sampler2D texture2; 

void main()
{
    // fragColor = vec4(customColor, 1.); // use a single colour
    // fragColor = texture(texture1, texCoord); // use a texture
    fragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.5); // combine texture and colour
    // fragColor = 0.5 * texture(texture1, texCoord) + 0.5 * texture(texture2, texCoord); // same effect as above
}