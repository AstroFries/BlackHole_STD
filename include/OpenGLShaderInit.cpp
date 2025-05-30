#include "OpenGLShaderInit.h"
#include <fstream>
#include <sstream>
#include <iostream>

GLuint compileShaderFromFile(const char* path, GLenum type) {
    std::ifstream fin(path);
    if (!fin) {
        std::cerr << "Shader file not found: " << path << std::endl;
        return 0;
    }
    std::stringstream ss;
    ss << fin.rdbuf();
    std::string src = ss.str();
    GLuint shader = glCreateShader(type);
    const char* csrc = src.c_str();
    glShaderSource(shader, 1, &csrc, nullptr);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Shader Compile Error (" << path << "): " << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint createProgram(const std::vector<GLuint>& shaders) {
    GLuint program = glCreateProgram();
    for (auto s : shaders) glAttachShader(program, s);
    glLinkProgram(program);
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "Program Link Error: " << infoLog << std::endl;
        glDeleteProgram(program);
        return 0;
    }
    for (auto s : shaders) glDetachShader(program, s);
    return program;
}

GLuint LoadShader(const std::string& vertexPath, const std::string& fragmentPath) {
    GLuint vShader = compileShaderFromFile(vertexPath.c_str(), GL_VERTEX_SHADER);
    GLuint fShader = compileShaderFromFile(fragmentPath.c_str(), GL_FRAGMENT_SHADER);

    if (vShader == 0 || fShader == 0) {
        std::cerr << "Failed to compile one or more shaders." << std::endl;
        glDeleteShader(vShader);
        glDeleteShader(fShader);
        return 0;
    }

    std::vector<GLuint> shaders = {vShader, fShader};
    GLuint program = createProgram(shaders);

    // 删除中间的 shader 对象（已链接进 program）
    glDeleteShader(vShader);
    glDeleteShader(fShader);

    return program;
}