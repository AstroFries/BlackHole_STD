#pragma once
#include <vector>
#include <Eigen>
#include <glad/glad.h>

class GeodesicComputeSHader {
public:
    GeodesicComputeSHader(int width, int height, int maxPaths = 32);
    ~GeodesicComputeSHader();

    // 编译和初始化
    bool init(const char* computeShaderPath);

    // 设置相机参数和其他 uniform 参数
    void setCameraUniforms(const Eigen::Vector3f& cameraPos, float stepSize, int nSamples, float theta, float phi);

    // 启动计算
    void dispatch();

    // 获取 SSBO，用于传递给下一个 shader 阶段
    GLuint getSampleBuffer() const { return sampleBufferSSBO_; }

    float pos[3], theta, phi, x_sight, y_sight;
    int width_, height_;
    int n_sample = 32;

private:
    int maxPaths_;
    GLuint program_ = 0;
    GLuint sampleBufferSSBO_ = 0; // 存储所有采样点的 SSBO
};