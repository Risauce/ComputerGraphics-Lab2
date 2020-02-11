#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out vec3 myColor;
uniform mat3 theMatrix; 

void main() {
    vec3 conversion = vec3(aPos, 1.0f);
    gl_Position = vec4((theMatrix * conversion), 1.0f);
    myColor = aColor;
}
