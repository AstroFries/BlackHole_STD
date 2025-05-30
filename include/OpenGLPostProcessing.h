#pragma once
#include <glad/glad.h>

#pragma once
#include <glad/glad.h>

class Bloom_GL {
public:
    Bloom_GL(int width, int height);
    ~Bloom_GL();

    // 初始化shader和FBO等资源
    bool init();

    // 设置输入纹理
    void setInputTexture(GLuint textureID);

    // 执行bloom处理，返回最终输出纹理
    GLuint process();

    // 可选：调整bloom强度、模糊迭代次数等参数
    void setBloomStrength(float strength);
    void setBlurIterations(int iterations);

private:
    int m_width, m_height;
    GLuint m_inputTex = 0;
    GLuint m_brightTex = 0;          // 高亮提取结果
    GLuint m_pingpongTex[2] = {0, 0}; // ping-pong模糊纹理
    GLuint m_fbo = 0;
    GLuint m_pingpongFBO[2] = {0, 0};

    GLuint m_extractShader = 0;
    GLuint m_blurShader = 0;
    GLuint m_combineShader = 0;
    GLuint m_quadVAO = 0, m_quadVBO = 0;
    float m_bloomStrength = 1.0f;
    int m_blurIterations = 8;

    // 内部辅助
    bool createResources();
    void releaseResources();
};