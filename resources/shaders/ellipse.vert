#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1)in vec2 aUV;

uniform vec2 uResolution;

out vec2 uv;

void main(){
    vec3 pos = aPos;

    float aspectRatio = uResolution.y / uResolution.x;
    pos.x *= aspectRatio;

    gl_Position = vec4(pos, 1.0);
    uv = aUV;
}