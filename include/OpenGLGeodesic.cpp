#include "OpenGLGeodesic.h"
#include "OpenGLShaderInit.h" // 你自己的 shader 工具函数，假设包含 compileShaderFromFile 和 createProgram
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>


GeodesicComputeSHader::GeodesicComputeSHader(int width, int height, int maxPaths)
    : width_(width), height_(height), maxPaths_(maxPaths) {}

GeodesicComputeSHader::~GeodesicComputeSHader() {
    if (program_) glDeleteProgram(program_);
    if (sampleBufferSSBO_) glDeleteBuffers(1, &sampleBufferSSBO_);
}

bool GeodesicComputeSHader::init(const char* computeShaderPath) {
    GLuint cs = compileShaderFromFile(computeShaderPath, GL_COMPUTE_SHADER);
    if (!cs) return false;
    program_ = createProgram({cs});
    glDeleteShader(cs);

    // 创建 SSBO 来保存所有采样点
    glGenBuffers(1, &sampleBufferSSBO_);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, sampleBufferSSBO_);

    size_t totalSamples = static_cast<size_t>(width_) * static_cast<size_t>(height_) * static_cast<size_t>(n_sample);
    glBufferData(GL_SHADER_STORAGE_BUFFER, totalSamples * 4 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, sampleBufferSSBO_);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // 解绑

    GLint success;
    glGetProgramiv(program_, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program_, 512, NULL, infoLog);
        std::cout << "Shader Link Error: " << infoLog << std::endl;
        return false;
    }

    return true;
}

void GeodesicComputeSHader::setCameraUniforms(const Eigen::Vector3f& cameraPos, float stepSize, int nSamples, float theta, float phi) {
    glUseProgram(program_);
    glUniform3f(glGetUniformLocation(program_, "cameraPos"), cameraPos.x(), cameraPos.y(), cameraPos.z());
    glUniform1f(glGetUniformLocation(program_, "stepSize"), stepSize);
    glUniform1i(glGetUniformLocation(program_, "n_sample"), nSamples);
    glUniform1f(glGetUniformLocation(program_, "cameraTheta"), theta);
    glUniform1f(glGetUniformLocation(program_, "cameraPhi"), phi);
    glUniform1i(glGetUniformLocation(program_, "screenWidth"), width_);
    glUniform1i(glGetUniformLocation(program_, "screenHeight"), height_);
}

void GeodesicComputeSHader::dispatch() {
    glUseProgram(program_);

    // 绑定 SSBO 到 binding=0
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, sampleBufferSSBO_);

    // 分发计算任务
    glDispatchCompute((width_ + 7) / 8, (height_ + 7) / 8, 1);

    // 内存屏障确保写入完成
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    // 解绑
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
}