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

    void updatePixels(const std::vector<unsigned char>& pixels);//用<unsigned char>更新屏幕
    void updatePixels(GLuint textureId);//用texture更新屏幕
    void render(bool if_swap = true);//渲染
    bool shouldClose();//检测关闭
    GLuint get_VAO(){return VAO;};

    void initShaders_Bloom(const char* shaderDIR);//输入shader的位置
    void enableBloom(bool enable){bloomEnabled = enable;};// 启用/禁用泛光
    void setBloomThreshold(float threshold);//泛光开始值
    void setBloomStrength(float strength) { this->bloomStrength = strength;};//泛光强度

    GLFWwindow* getWindow() { return window; }

    int bloom_N = 16;//ping pong 次数,2次相当于一次高斯卷积
    
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
    

    void initPostProcessFramebuffers();
    void extractBrightAreas(GLuint sourceTexture);//截取高亮区
    void gaussianBlur(GLuint sourceTexture, int iterations = 4);//pingpong高斯卷积
    void combineFinalImage(GLuint originalTexture, GLuint bloomTexture);//将泛光图叠加到原图上
    void RenderQuad();//更新
};

#endif // OPENGL_RENDERER_H