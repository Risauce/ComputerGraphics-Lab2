#version 330 core

in vec3 myColor; 
layout(location = 0)out vec4 fragColor;

void main() {
    fragColor = vec4(myColor, 1.0f);
}
