#pragma once
#include <vector>
#include <Eigen>
#include <glad/glad.h>

class VolumeComputeShader {
public:
    VolumeComputeShader(int width, int height, int maxPaths = 64);
    ~VolumeComputeShader();

    // ����ͳ�ʼ��
    bool init(const char* computeShaderPath);

    // �ϴ�·�����ݣ�ÿ�������maxPaths����ÿ��vec4(x,y,z,length)��
    void uploadPaths(const std::vector<std::vector<Eigen::Vector4f>>& allPaths);

    // ��������
    void dispatch();

    // ��ȡ���
    void downloadPower(std::vector<float>& powerOut);

    GLuint getPowerTexture() const { return powerTex_; }

private:
    int width_, height_, maxPaths_;
    GLuint program_ = 0;
    GLuint pathSSBO_ = 0;
    GLuint powerTex_ = 0;
};