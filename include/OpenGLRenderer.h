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
    void render();
    bool shouldClose();

private:
    void initWindow(const std::string& title);
    void initShaders();
    void initBuffers();

    GLFWwindow* window;
    GLuint texture;
    GLuint VAO, VBO, EBO;
    GLuint shaderProgram;

    int width, height;
};

#endif // OPENGL_RENDERER_H