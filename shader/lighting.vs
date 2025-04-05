	#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
 
uniform mat4 transform;
uniform mat4 modelTransform; //Local 좌표계 -> World 좌표계로 바꾸는 행렬
 
out vec3 normal;
out vec2 texCoord;
out vec3 position;
 
void main() {
    gl_Position = transform * vec4(aPos, 1.0);
    normal = (transpose(inverse(modelTransform)) * vec4(aNormal, 0.0)).xyz;
    texCoord = aTexCoord;
    position = (modelTransform * vec4(aPos, 1.0)).xyz; //World 좌표계
}