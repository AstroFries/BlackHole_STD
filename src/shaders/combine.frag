#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float bloomStrength;
uniform bool bloom;

void main() {
    vec3 result = texture(scene, TexCoord).rgb;
    if(bloom)
        result += texture(bloomBlur, TexCoord).rgb * bloomStrength;
    FragColor = vec4(result, 1.0);
}