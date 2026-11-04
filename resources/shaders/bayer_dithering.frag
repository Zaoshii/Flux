
#version 460 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;

const float bayerMatrix[16] = float[](
     0.0,  8.0,  2.0, 10.0,
    12.0,  4.0, 14.0,  6.0,
     3.0, 11.0,  1.0,  9.0,
    15.0,  7.0, 13.0,  5.0
);

float getBayerValue(ivec2 pixel)
{
    int x = pixel.x % 4;
    int y = pixel.y % 4;

    return bayerMatrix[x + y * 4] / 16.0;
}

void main()
{
    vec4 color = texture(screenTexture, TexCoord);

    // Bayer threshold
    float threshold = getBayerValue(ivec2(gl_FragCoord.xy));

    // levels per color channel
    float levels = 1.75;

    // dithering
    vec3 dithered = floor(color.rgb * levels + threshold) / levels;

    FragColor = vec4(clamp(dithered, 0.0, 1.0), color.a);
}
