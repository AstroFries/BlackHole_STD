#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <functional>
#include <stdexcept>
#include <cmath>
#include <iostream>

class OpenGLConvolver {
public:
    // ���캯������ʼ��OpenGL��������Դ
    OpenGLConvolver(int width, int height) 
        : width(width), height(height), window(nullptr), shaderProgram(0) {
        initOpenGL();
        initShaders();
        initBuffers();
    }

    // ����������������Դ
    ~OpenGLConvolver() {
        cleanup();
    }

    // ִ�о������
    std::vector<std::vector<float>> convolve(
        const std::function<float(float, float)>& func,
        const std::vector<std::vector<float>>& kernel) {
        
        // 1. �Ӻ���������������
        auto inputData = generateInputData(func);
        
        // 2. ������������
        GLuint inputTexture = createInputTexture(inputData);
        
        // 3. ׼�������
        prepareKernel(kernel);
        
        // 4. ִ�о��
        GLuint resultTexture = performConvolution(inputTexture, kernel.size(), kernel[0].size());
        
        // 5. ��ȡ���
        auto result = readResultTexture(resultTexture);
        
        // 6. ������ʱ��Դ
        glDeleteTextures(1, &inputTexture);
        glDeleteTextures(1, &resultTexture);
        
        return result;
    }

private:
    int width, height;
    GLFWwindow* window;
    GLuint shaderProgram;
    GLuint quadVAO, quadVBO;
    GLuint pingpongFBO[2];
    GLuint pingpongTextures[2];

    // ��ʼ��OpenGL����
    void initOpenGL() {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // ���ش���

        window = glfwCreateWindow(width, height, "OpenGL Convolver", NULL, NULL);
        if (!window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(window);
        
        // ʹ��GLAD����OpenGL����ָ��
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            glfwTerminate();
            throw std::runtime_error("Failed to initialize GLAD");
        }
    }

    // ��ʼ����ɫ��
    void initShaders() {
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

        const char* fragmentShaderSource = R"(
            #version 330 core
            in vec2 TexCoord;
            out vec4 FragColor;
            uniform sampler2D inputTexture;
            uniform vec2 texelSize;
            uniform float kernel[25]; // ֧�����5x5��
            uniform int kernelWidth;
            uniform int kernelHeight;

            void main() {
                vec3 result = vec3(0.0);
                int halfWidth = kernelWidth / 2;
                int halfHeight = kernelHeight / 2;
                
                for (int y = -halfHeight; y <= halfHeight; ++y) {
                    for (int x = -halfWidth; x <= halfWidth; ++x) {
                        vec2 offset = vec2(x, y) * texelSize;
                        float weight = kernel[(y + halfHeight) * kernelWidth + (x + halfWidth)];
                        result += texture(inputTexture, TexCoord + offset).rgb * weight;
                    }
                }
                
                FragColor = vec4(result, 1.0);
            }
        )";

        // ���붥����ɫ��
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        checkShaderCompile(vertexShader, "VERTEX");

        // ����Ƭ����ɫ��
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        checkShaderCompile(fragmentShader, "FRAGMENT");

        // ������ɫ������
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        checkProgramLink(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    // ��ʼ��������������
    void initBuffers() {
        // �ı��ζ�������
        float quadVertices[] = {
            -1.0f,  1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
             1.0f, -1.0f, 1.0f, 0.0f,
            
            -1.0f,  1.0f, 0.0f, 1.0f,
             1.0f, -1.0f, 1.0f, 0.0f,
             1.0f,  1.0f, 1.0f, 1.0f
        };

        // ����VAO��VBO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        // ����ƹ�һ���֡���������
        glGenFramebuffers(2, pingpongFBO);
        glGenTextures(2, pingpongTextures);
        
        for (unsigned int i = 0; i < 2; i++) {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
            glBindTexture(GL_TEXTURE_2D, pingpongTextures[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongTextures[i], 0);
            
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                throw std::runtime_error("Framebuffer is not complete!");
            }
        }
    }

    // �Ӻ���������������
    std::vector<float> generateInputData(const std::function<float(float, float)>& func) {
        std::vector<float> data(width * height);
        
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                // �����������һ����[-1,1]��Χ
                float nx = (2.0f * x / width) - 1.0f;
                float ny = (2.0f * y / height) - 1.0f;
                data[y * width + x] = func(nx, ny);
            }
        }
        
        return data;
    }

    // ������������
    GLuint createInputTexture(const std::vector<float>& data) {
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, data.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        return texture;
    }

    // ׼�������
    void prepareKernel(const std::vector<std::vector<float>>& kernel) {
        if (kernel.empty() || kernel[0].empty()) {
            throw std::runtime_error("Kernel cannot be empty");
        }
        
        if (kernel.size() > 5 || kernel[0].size() > 5) {
            throw std::runtime_error("Kernel size too large (max 5x5 supported)");
        }
        
        // չƽ�˲��ϴ�����ɫ��
        std::vector<float> flatKernel(25, 0.0f); // ��ʼ��Ϊ0
        for (size_t y = 0; y < kernel.size(); ++y) {
            for (size_t x = 0; x < kernel[y].size(); ++x) {
                flatKernel[y * 5 + x] = kernel[y][x];
            }
        }
        
        glUseProgram(shaderProgram);
        glUniform1fv(glGetUniformLocation(shaderProgram, "kernel"), 25, flatKernel.data());
        glUniform1i(glGetUniformLocation(shaderProgram, "kernelWidth"), static_cast<int>(kernel[0].size()));
        glUniform1i(glGetUniformLocation(shaderProgram, "kernelHeight"), static_cast<int>(kernel.size()));
        glUniform2f(glGetUniformLocation(shaderProgram, "texelSize"), 1.0f/width, 1.0f/height);
    }

    // ִ�о��
    GLuint performConvolution(GLuint inputTexture, int kernelWidth, int kernelHeight) {
        // ����������
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, inputTexture);
        
        // ��Ⱦ����һ��ƹ�һ���
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[0]);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        glUniform1i(glGetUniformLocation(shaderProgram, "inputTexture"), 0);
        
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        
        // ���ؽ������
        return pingpongTextures[0];
    }

    // �������ȡ���
    std::vector<std::vector<float>> readResultTexture(GLuint texture) {
        std::vector<float> pixels(width * height);
        glBindTexture(GL_TEXTURE_2D, texture);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, pixels.data());
        
        // ������֯Ϊ2D����
        std::vector<std::vector<float>> result(height, std::vector<float>(width));
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                result[y][x] = pixels[y * width + x];
            }
        }
        
        return result;
    }

    // ������Դ
    void cleanup() {
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
        glDeleteTextures(2, pingpongTextures);
        glDeleteFramebuffers(2, pingpongFBO);
        glDeleteProgram(shaderProgram);
        
        if (window) {
            glfwDestroyWindow(window);
            glfwTerminate();
        }
    }

    // �����ɫ���������
    void checkShaderCompile(GLuint shader, const std::string& type) {
        GLint success;
        GLchar infoLog[1024];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            throw std::runtime_error("ERROR::SHADER_COMPILATION_ERROR of type: " + type + "\n" + infoLog);
        }
    }

    // ���������Ӵ���
    void checkProgramLink(GLuint program) {
        GLint success;
        GLchar infoLog[1024];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 1024, NULL, infoLog);
            throw std::runtime_error("ERROR::PROGRAM_LINKING_ERROR\n");
        }
    }
};