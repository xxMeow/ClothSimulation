#version 330 core

out vec4 color;

in vec2 texCoord;

// Texture Sampler
uniform sampler2D uniTex;

void main()
{
    color = texture(uniTex, texCoord);
    // texture() will output the color obtained by sampling the texture with configured conditions
}
