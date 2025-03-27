#version 330 core
layout (location = 0) in vec3 aPos;
// location = 0 이 곧 VAO attribute 0 번을 의미

// 벡터값을 변경하여 정점의 위치를 바꿀 수 있다.
void main() {
    gl_Position = vec4(aPos, 1.0);
}