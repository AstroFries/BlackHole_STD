#pragma once
#include <vector>
#include <Eigen>
#include <glad/glad.h>

class VolumeComputeShader {
public:
    VolumeComputeShader(int width, int height, int maxPaths = 32);
    ~VolumeComputeShader();

    // 编译和初始化
    bool init(const char* computeShaderPath);

    // 上传路径数据（每像素最多maxPaths条，每条vec4(x,y,z,length)）
    void uploadPaths(const std::vector<std::vector<Eigen::Vector4f>>& allPaths);

    // 启动计算
    void dispatch();

    // 读取结果
    void downloadPower(std::vector<float>& powerOut);
    GLuint getPowerTexture() const { return powerTex_; }

    float T_ = 5000, L_ = 1e6, time_ = 0;
    bool if_dopplerT_ = true, if_dopplerI_ = true;


private:
    int width_, height_, maxPaths_;
    GLuint program_ = 0;
    GLuint pathSSBO_ = 0;
    GLuint powerTex_ = 0;
};