#version 330 core

layout(location = 0) in vec3 aPos; // 顶点位置

void main() {
    gl_Position = vec4(aPos, 1.0); // 将顶点位置传递到裁剪空间
}