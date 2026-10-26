#version 460 core

layout (location = 0) in vec3 aPos;
//layout (location = 1)in vec2 aUV;

uniform float uTime;


//out vec2 uv;

void main(){
    gl_Position = vec4(cos(aPos.x + uTime), sin(aPos.y + 2.0 * uTime), 0.0, 1.0);
    //uv = aUV;
}