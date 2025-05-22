#include "OpenGLVolumeRender.h"
#include <fstream>
#include <sstream>
#include <iostream>

static GLuint compileShader(const char* path, GLenum type) {
    std::ifstream fin(path);
    std::stringstream ss;
    ss << fin.rdbuf();
    std::string src = ss.str();
    GLuint shader = glCreateShader(type);
    const char* csrc = src.c_str();
    glShaderSource(shader, 1, &csrc, nullptr);
    glCompileShader(shader);
    // 检查编译错误...
    return shader;
}

VolumeComputeShader::VolumeComputeShader(int width, int height, int maxPaths)
    : width_(width), height_(height), maxPaths_(maxPaths) {}

VolumeComputeShader::~VolumeComputeShader() {
    if (program_) glDeleteProgram(program_);
    if (pathSSBO_) glDeleteBuffers(1, &pathSSBO_);
    if (powerTex_) glDeleteTextures(1, &powerTex_);
}

bool VolumeComputeShader::init(const char* computeShaderPath) {
    GLuint cs = compileShader(computeShaderPath, GL_COMPUTE_SHADER);
    program_ = glCreateProgram();
    glAttachShader(program_, cs);
    glLinkProgram(program_);
    glDeleteShader(cs);

    // 创建SSBO
    glGenBuffers(1, &pathSSBO_);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, pathSSBO_);
    glBufferData(GL_SHADER_STORAGE_BUFFER, width_ * height_ * maxPaths_ * sizeof(float) * 4, nullptr, GL_DYNAMIC_DRAW);

    // 创建输出纹理
    glGenTextures(1, &powerTex_);
    glBindTexture(GL_TEXTURE_2D, powerTex_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width_, height_, 0, GL_RGBA, GL_FLOAT, nullptr);
    GLint success;
    glGetProgramiv(program_, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program_, 512, NULL, infoLog);
        std::cout << "Shader Link Error: " << infoLog << std::endl;
    }
    return true;
}

void VolumeComputeShader::uploadPaths(const std::vector<std::vector<Eigen::Vector4f>>& allPaths) {
    std::vector<Eigen::Vector4f> flat(width_ * height_ * maxPaths_, Eigen::Vector4f(0,0,0,0));
    for (int i = 0; i < width_ * height_; ++i) {
        for (int j = 0; j < allPaths[i].size() && j < maxPaths_; ++j) {
            flat[i * maxPaths_ + j] = allPaths[i][j];
        }
    }
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, pathSSBO_);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, flat.size() * sizeof(Eigen::Vector4f), flat.data());
}

void VolumeComputeShader::dispatch() {
    glUseProgram(program_);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, pathSSBO_);
    glBindImageTexture(1, powerTex_, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glUniform1i(glGetUniformLocation(program_, "width"), width_);
    glUniform1i(glGetUniformLocation(program_, "height"), height_);
    glUniform1i(glGetUniformLocation(program_, "pathStride"), maxPaths_);
    glDispatchCompute((width_+15)/16, (height_+15)/16, 1);
    glBindImageTexture(1, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    // 解绑 SSBO
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void VolumeComputeShader::downloadPower(std::vector<float>& powerOut) {
    powerOut.resize(width_ * height_);
    std::vector<float> tmp(width_ * height_ * 4);
    glBindTexture(GL_TEXTURE_2D, powerTex_);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, tmp.data());
    for (int i = 0; i < width_ * height_; ++i){
        for (int j = 0; j < 3; ++j)powerOut[3 * i + j] = tmp[i*4 + j]; 
    }
}