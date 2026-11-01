#version 460 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;

void main()
{
    vec4 color = texture(screenTexture, TexCoord);

    float levels = 1.25;

    vec3 posterized = floor(color.rgb * levels) / levels;

    FragColor = vec4(posterized, color.a);
}
