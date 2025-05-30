#pragma once
#include <glad/glad.h>
#include <string>
#include <vector>

// ���뵥��shader
GLuint compileShaderFromFile(const char* path, GLenum type);

// ����program���ɱ������֧�ֶ��shader��
GLuint createProgram(const std::vector<GLuint>& shaders);

GLuint LoadShader(const std::string& vertexPath, const std::string& fragmentPath);