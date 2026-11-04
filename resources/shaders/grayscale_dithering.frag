
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

float quantitize(float x, float threshold)
{
    float t = x;
    if (x < threshold)
    {
        float t = float(floor(x));
    }
    else if (x >= threshold)
    {
        float t = float(ceil(x));
    }
    return t;
}

float getBayerValue(ivec2 pixel)
{
    int x = pixel.x % 4;
    int y = pixel.y % 4;

    return bayerMatrix[x + y * 4]/16.0;
}

float lum(vec3 rgb)
{
    return dot(rgb,vec3(0.2126, 0.7152, 0.0722));
}

void main()
{
    float levels = 4.0;

    vec4 color = texture(screenTexture, TexCoord);

    float luminance = lum(color.rgb);

    float threshold = getBayerValue(ivec2(gl_FragCoord.xy));

    float finalColor = luminance > threshold ? 1.0 : 0.0;

    FragColor = vec4(vec3(finalColor),1.0);
}
