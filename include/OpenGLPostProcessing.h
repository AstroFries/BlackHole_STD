#pragma once
#include <glad/glad.h>

#pragma once
#include <glad/glad.h>

class Bloom_GL {
public:
    Bloom_GL(int width, int height);
    ~Bloom_GL();

    // ��ʼ��shader��FBO����Դ
    bool init();

    // ������������
    void setInputTexture(GLuint textureID);

    // ִ��bloom�������������������
    GLuint process();

    // ��ѡ������bloomǿ�ȡ�ģ�����������Ȳ���
    void setBloomStrength(float strength);
    void setBlurIterations(int iterations);

private:
    int m_width, m_height;
    GLuint m_inputTex = 0;
    GLuint m_brightTex = 0;          // ������ȡ���
    GLuint m_pingpongTex[2] = {0, 0}; // ping-pongģ������
    GLuint m_fbo = 0;
    GLuint m_pingpongFBO[2] = {0, 0};

    GLuint m_extractShader = 0;
    GLuint m_blurShader = 0;
    GLuint m_combineShader = 0;
    GLuint m_quadVAO = 0, m_quadVBO = 0;
    float m_bloomStrength = 1.0f;
    int m_blurIterations = 8;

    // �ڲ�����
    bool createResources();
    void releaseResources();
};