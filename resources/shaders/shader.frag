#version 460 core

out vec4 FragColor;

uniform float uTime;
uniform vec2 uResolution;

in vec4 gl_FragCoord;

//in vec2 uv;

void main(){
    vec2 uv = gl_FragCoord.xy / uResolution;
    FragColor = vec4(abs(cos(2.0 * uTime))* uv.x, abs(sin(2.0 * uTime))* uv.y, abs(cos(sin(2.0 * uTime)))*((uv.x+uv.y)/2.0), 1.0);
}