#version 330 core

uniform sampler2D terrain0;

in vec2 fragUV;

out vec3 color;

void main(){
  color = texture(terrain0, fragUV).rgb;
}
