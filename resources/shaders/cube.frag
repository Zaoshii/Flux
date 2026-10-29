#version 460 core

#define PI 3.141592653589793
#define HALF_PI 1.5707963267948966

out vec4 FragColor;

in vec2 uv;
in vec3 normal;

float cosr(float x, float translation){
    return (cos(2.0 * PI * ( x - (translation / 3.0)))+1.0)/2.0;
}

void main(){
    FragColor = vec4(cosr(uv.x,0.0),cosr(uv.x,1.0),cosr(uv.x,2.0),1.0);
    //FragColor = vec4(1.0,0.0,0.0,1.0);
}