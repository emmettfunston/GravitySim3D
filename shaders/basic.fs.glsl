#version 330 core
uniform vec3 bodyColor;
out vec4 FragColor;
void main(){
    FragColor=vec4(bodyColor,1.0);
}  