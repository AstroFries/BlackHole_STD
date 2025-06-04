#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D scene;
uniform float brightnessThreshold;

void main() {
    vec3 color = texture(scene, TexCoord).rgb;
    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722)); // 计算亮度
    if (brightness >= brightnessThreshold)
        FragColor = vec4(color, 1.0);
    else
        FragColor = vec4(color * pow(brightness / brightnessThreshold,1), 1.0); // 非亮部输出一部分
}