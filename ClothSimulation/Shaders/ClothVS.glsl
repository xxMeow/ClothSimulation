#version 330 core

layout (location = 0) in vec3 vsPosition;
layout (location = 1) in vec2 vsTexCoord;
layout (location = 2) in vec3 vsNormal;

out vec3 position;
out vec2 texCoord;
out vec3 normal;

uniform mat4 uniModelMatrix;
uniform mat4 uniViewMatrix;
uniform mat4 uniProjMatrix;

void main()
{
    position = vsPosition;
    normal = vsNormal;
    gl_Position = uniProjMatrix * uniViewMatrix * uniModelMatrix * vec4(vsPosition, 1.0f);
    texCoord = vec2(vsTexCoord.x, vsTexCoord.y);
}
