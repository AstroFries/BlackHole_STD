#include "OpenGLRenderer.h"
#include "OpenGLShaderInit.h"
#include <fstream>
#include <sstream>
#include <iostream>

// 顶点着色器源码
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec2 aTexCoord;

    out vec2 TexCoord;

    void main() {
        gl_Position = vec4(aPos, 0.0, 1.0);
        TexCoord = aTexCoord;
    }
)";

// 片段着色器源码
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    in vec2 TexCoord;

    uniform sampler2D texture1;

    void main() {
        FragColor = texture(texture1, TexCoord);
    }
)";

OpenGLRenderer::OpenGLRenderer(int width, int height, const std::string& title)
    : width(width), height(height) {
    initWindow(title);
    initShaders();
    initBuffers();
    initPostProcessFramebuffers();
}

OpenGLRenderer::~OpenGLRenderer() {
    glDeleteTextures(1, &texture);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwDestroyWindow(window);
    glfwTerminate();
}

void OpenGLRenderer::initWindow(const std::string& title) {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glViewport(0, 0, width, height);
}

void OpenGLRenderer::initShaders() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);

    
}

void OpenGLRenderer::initShaders_Bloom(const char* shaderDIR){
    std::string basePath = shaderDIR;
    blurShader = LoadShader((basePath + "/blur.vert").c_str(), (basePath + "/blur.frag").c_str());
    combineShader = LoadShader((basePath + "/combine.vert").c_str(), (basePath + "/combine.frag").c_str());
    brightShader = LoadShader((basePath + "/bright.vert").c_str(), (basePath + "/bright.frag").c_str());

    if (!blurShader || !combineShader) {
        throw std::runtime_error("Failed to load post-processing shaders.");
    }
}

void OpenGLRenderer::initBuffers() {
    float vertices[] = {
        // positions   // texture coords
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f,
        -1.0f,  1.0f,  0.0f, 1.0f
    };
    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void OpenGLRenderer::updatePixels(const std::vector<unsigned char>& pixels) {
    glUseProgram(shaderProgram);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
}

void OpenGLRenderer::updatePixels(GLuint textureId) {
    extTexture = textureId;
}

void OpenGLRenderer::render() {
    glClear(GL_COLOR_BUFFER_BIT);

    GLuint texToUse = extTexture ? extTexture : texture;

    if (bloomEnabled) {
        extractBrightAreas(texToUse);
        gaussianBlur(brightTexture, bloom_N);
        combineFinalImage(texToUse, pingpongTex[1]);
    } else {
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texToUse);
        glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
        RenderQuad();
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool OpenGLRenderer::shouldClose() {
    return glfwWindowShouldClose(window);
}

void OpenGLRenderer::initPostProcessFramebuffers() {
    glGenFramebuffers(1, &brightFBO); 
    glGenTextures(1, &brightTexture);

    glBindTexture(GL_TEXTURE_2D, brightTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, brightFBO); // 使用 brightFBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brightTexture, 0);

    // 检查是否完整
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "brightFBO not complete!" << std::endl;

    // 初始化 pingpong FBO（这部分保留不变）
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongTex);

    for (int i = 0; i < 2; ++i) {
        glBindTexture(GL_TEXTURE_2D, pingpongTex[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongTex[i], 0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // 恢复默认帧缓冲
}

void OpenGLRenderer::RenderQuad() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void OpenGLRenderer::extractBrightAreas(GLuint sourceTexture) {

    glBindFramebuffer(GL_FRAMEBUFFER, brightFBO);
    glUseProgram(brightShader);

    glUniform1f(glGetUniformLocation(brightShader, "brightnessThreshold"), bloomThreshold);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sourceTexture);
    glUniform1i(glGetUniformLocation(brightShader, "scene"), 0);

    RenderQuad(); // 绘制全屏 Quad

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLRenderer::gaussianBlur(GLuint sourceTexture, int iterations) {
    bool horizontal = true;
    for (int i = 0; i < iterations; ++i) {
        int index = i % 2;
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[index]);
        glUseProgram(blurShader);
        glUniform1i(glGetUniformLocation(blurShader, "horizontal"), horizontal);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, horizontal ? sourceTexture : pingpongTex[1 - index]);

        RenderQuad();
        horizontal = !horizontal;
    }
}

void OpenGLRenderer::combineFinalImage(GLuint originalTexture, GLuint bloomTexture) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // 渲染到屏幕
    glUseProgram(combineShader);

    glUniform1i(glGetUniformLocation(combineShader, "bloom"), true);
    glUniform1f(glGetUniformLocation(combineShader, "bloomStrength"), bloomStrength);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, originalTexture);
    glUniform1i(glGetUniformLocation(combineShader, "scene"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, bloomTexture);
    glUniform1i(glGetUniformLocation(combineShader, "bloomBlur"), 1);

    RenderQuad();
}
