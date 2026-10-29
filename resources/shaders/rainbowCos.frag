#version 460 core

#define PI 3.141592653589793
#define HALF_PI 1.5707963267948966

out vec4 FragColor;

uniform float uTime;
uniform vec2 uResolution;
//in vec4 gl_FragCoord;

in vec2 uv;

float cosru(float x, float translation, float time, float md){
    return (cos( 2.0 * time + 2.0 * PI * (mod(time, md) * x - (translation / 3.0)))+1.0)/2.0;
}

float cosr(float x, float translation){
    return (cos(2.0 * PI * ( x - (translation / 3.0)))+1.0)/2.0;
}


/*void main(){
    FragColor = vec4(cosru(uv.x, 0, 2.0 * uTime, 6.5),cosru(uv.y, 1, 2.5 * uTime, 15.4),cosru((uv.x+uv.y)/2.0, 2, 2.25 * uTime, 11.2), 1.0);
    //FragColor = vec4(1.0,1.0,0.0,1.0);
}*/

void main(){
    FragColor = vec4(cosr(uv.y + uTime,0.0), cosr(uv.y + uTime,1.0), cosr(uv.y + uTime,2.0), 1.0);
}