#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

class OpenGLRenderer {
public:
    OpenGLRenderer(int width, int height, const std::string& title);
    ~OpenGLRenderer();

    void updatePixels(const std::vector<unsigned char>& pixels);//��<unsigned char>������Ļ
    void updatePixels(GLuint textureId);//��texture������Ļ
    void render(bool if_swap = true);//��Ⱦ
    bool shouldClose();//���ر�
    GLuint get_VAO(){return VAO;};

    void initShaders_Bloom(const char* shaderDIR);//����shader��λ��
    void enableBloom(bool enable){bloomEnabled = enable;};// ����/���÷���
    void setBloomThreshold(float threshold);//���⿪ʼֵ
    void setBloomStrength(float strength) { this->bloomStrength = strength;};//����ǿ��

    GLFWwindow* getWindow() { return window; }

    int bloom_N = 16;//ping pong ����,2���൱��һ�θ�˹���
    
private:
    void initWindow(const std::string& title);
    void initShaders();
    void initBuffers();

    GLFWwindow* window;
    GLuint texture;
    GLuint VAO, VBO, EBO;
    GLuint shaderProgram;
    GLuint extTexture = 0;

    int width, height;


    //����
    GLuint brightTexture;                // ��ȡ����������
    GLuint pingpongFBO[2], pingpongTex[2]; // Ping-Pong FBO & Textures
    GLuint blurShader = 0, combineShader = 0, brightShader = 0;     // ��ɫ��
    GLuint brightFBO = 0;      // FBO ������Ⱦ����

    bool bloomEnabled = true;
    float bloomThreshold = 0.2f;
    float bloomStrength = 0.2f;
    

    void initPostProcessFramebuffers();
    void extractBrightAreas(GLuint sourceTexture);//��ȡ������
    void gaussianBlur(GLuint sourceTexture, int iterations = 4);//pingpong��˹���
    void combineFinalImage(GLuint originalTexture, GLuint bloomTexture);//������ͼ���ӵ�ԭͼ��
    void RenderQuad();//����
};

#endif // OPENGL_RENDERER_H