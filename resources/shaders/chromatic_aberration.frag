
#version 460 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;

void main()
{
    // Distance from the centre of the screen
    vec2 center = TexCoord - vec2(0.5);

    // intensity
    float intensity = 1.25;

    float strength = intensity * 0.01;

    // Sample each colour channel at a different position
    float r = texture(
        screenTexture,
        TexCoord + center * strength
    ).r;

    float g = texture(
        screenTexture,
        TexCoord
    ).g;

    float b = texture(
        screenTexture,
        TexCoord - center * strength
    ).b;

    FragColor = vec4(r, g, b, 1.0);
}
