#pragma once
#include <vector>
#include <Eigen>
#include <glad/glad.h>

class GeodesicComputeSHader {
public:
    GeodesicComputeSHader(int width, int height, int maxPaths = 32);
    ~GeodesicComputeSHader();

    // ����ͳ�ʼ��
    bool init(const char* computeShaderPath);

    // ����������������� uniform ����
    void setCameraUniforms(const Eigen::Vector3f& cameraPos, float stepSize, int nSamples, float theta, float phi);

    // ��������
    void dispatch();

    // ��ȡ SSBO�����ڴ��ݸ���һ�� shader �׶�
    GLuint getSampleBuffer() const { return sampleBufferSSBO_; }

    float pos[3], theta, phi, x_sight, y_sight;
    int width_, height_;
    int n_sample = 32;

private:
    int maxPaths_;
    GLuint program_ = 0;
    GLuint sampleBufferSSBO_ = 0; // �洢���в������ SSBO
};