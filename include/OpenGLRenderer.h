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

    void updatePixels(const std::vector<unsigned char>& pixels);
    void updatePixels(GLuint textureId);
    void render();
    bool shouldClose();
    GLuint get_VAO(){return VAO;};

    void initShaders_Bloom(const char* shaderDIR);
    void enableBloom(bool enable);       // 启用/禁用泛光
    void setBloomThreshold(float threshold);
    void setBloomStrength(float strength);

    
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


    //泛光
    GLuint brightTexture;                // 提取的亮部纹理
    GLuint pingpongFBO[2], pingpongTex[2]; // Ping-Pong FBO & Textures
    GLuint blurShader = 0, combineShader = 0, brightShader = 0;     // 着色器
    GLuint brightFBO = 0;      // FBO 用于渲染亮部

    bool bloomEnabled = true;
    float bloomThreshold = 0.2f;
    float bloomStrength = 0.2f;
    int bloom_N = 16;

    void initPostProcessFramebuffers();
    void extractBrightAreas(GLuint sourceTexture);
    void gaussianBlur(GLuint sourceTexture, int iterations = 4);
    void combineFinalImage(GLuint originalTexture, GLuint bloomTexture);
    void RenderQuad();
};

#endif // OPENGL_RENDERER_H