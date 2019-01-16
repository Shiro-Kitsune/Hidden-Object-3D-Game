#version 330 core

out vec4 FragColor;

// Interpolated values from the vertex shaders
in vec2 UV;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

void main()
{
    FragColor = texture2D(myTextureSampler, UV);
}