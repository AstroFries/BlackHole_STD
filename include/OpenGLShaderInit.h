#pragma once
#include <glad/glad.h>
#include <string>
#include <vector>

// 编译单个shader
GLuint compileShaderFromFile(const char* path, GLenum type);

// 创建program（可变参数，支持多个shader）
GLuint createProgram(const std::vector<GLuint>& shaders);

GLuint LoadShader(const std::string& vertexPath, const std::string& fragmentPath);