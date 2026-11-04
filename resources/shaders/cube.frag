#version 460 core

#define PI 3.141592653589793
#define HALF_PI 1.5707963267948966

out vec4 FragColor;

in vec2 uv;
in vec3 normal;

float cosr(float x, float translation){
    return (cos(2.0 * PI * ( x - (translation / 3.0)))+1.0)/2.0;
}

vec3 palette( in float t, in vec3 a, in vec3 b, in vec3 c, in vec3 d )
{
    return a + b*cos( 6.283185*(c*t+d) );
}

void main(){
    FragColor = vec4(cosr(uv.x,0.0),cosr(uv.x,1.0),cosr(uv.x,2.0),1.0);
    //vec3 a = vec3(0.558, 0.878, 0.338);
    //vec3 b = vec3(0.498, 0.538, 0.500);
    //vec3 c = vec3(0.878, 1.548, 0.288); 
    //vec3 d = vec3(1.838, -0.092, 0.808);
    //FragColor = vec4(palette(uv.x, a, b, c, d),1.0);
}