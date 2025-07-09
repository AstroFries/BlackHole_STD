#include "OpenGLRenderer.h"
#include "OpenGLVolumeRender.h"
#include "OpenGLGeodesic.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <Eigen>
#include <vector>
#include <memory>
#include <assert.h>
#include <chrono>
#include <cmath>
#include <iostream>
#include <algorithm>

#define X_SIGHT 2.0*0.6
#define Y_SIGHT 1.5*0.6
#define PATH_N 32
#define R 0.15

void test(GLuint ssbo){
    int N = 5; // 想要读取前 5 个 vec4 数据
    size_t bufferSize = N * sizeof(Eigen::Vector4f);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);

    // 确保 GPU 写入完成
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    // 只映射前 N 个点
    void* data = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, bufferSize, GL_MAP_READ_BIT);

    if (data) {
        std::vector<Eigen::Vector4f> firstNPoints(
            static_cast<Eigen::Vector4f*>(data),
            static_cast<Eigen::Vector4f*>(data) + N
        );

        for (int i = 0; i < N; ++i) {
            std::cout << "Point[" << i << "] = " 
                    << firstNPoints[i].transpose() << std::endl;
        }

        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    } else {
        std::cerr << "Failed to map buffer!" << std::endl;
    }
}
int main() {
    const int width = 400*4;
    const int height = 300*4;

    //Shader Init
    OpenGLRenderer renderer(width, height, "OpenGL Renderer");
    renderer.initShaders_Bloom(SHADERS_DIR);
    
    VolumeComputeShader volume_shader(width, height, PATH_N);
    volume_shader.init(SHADERS_DIR "/volume_compute.comp");

    GeodesicComputeSHader geodesic_shader(width, height, PATH_N);
    geodesic_shader.init(SHADERS_DIR "/geodesic_compute.comp");
    std::cout << "init finish" << std::endl;

    //Imgui
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(renderer.getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    //Set Camera Para
    geodesic_shader.setCameraUniforms(Eigen::Vector3f(0,0,-2),0.01,PATH_N, 0.00, 0.02);

    //Dispatch
    geodesic_shader.dispatch();
    std::cout << "dispatch finish" << std::endl;
    GLuint path = geodesic_shader.getSampleBuffer();
    test(path);
    

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    //Upload Path
    volume_shader.uploadPathsFromSSBO(path);

    //Main loop
    float time = 0;
    int time_int = 0;
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed;

    while (!renderer.shouldClose()) {
        if (1==1){
            geodesic_shader.setCameraUniforms(Eigen::Vector3f(-3.5*sin(time*3),0,-3.5*cos(time*3)),0.005,PATH_N, 0.00, time*3);

            //Dispatch
            geodesic_shader.dispatch();
            //std::cout << "dispatch finish" << std::endl;
            GLuint path = geodesic_shader.getSampleBuffer();
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
            //Upload Path
            volume_shader.uploadPathsFromSSBO(path);
        }

        volume_shader.dispatch();
        
        volume_shader.time_ = time*10;
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Control");
        static float strength = 0.2f, LM = 6.f;
        if (ImGui::SliderFloat("Bloom Strength", &strength, 0.0f, 2.0f))
            renderer.setBloomStrength(strength);
        static float T = 5000.f;
        if (ImGui::SliderFloat("Tempetarue", &T, 500.0f, 20000.0f))
            volume_shader.T_ = T;
        static int if_dopplerI = true, if_dopplerT = true, bloom_N = 8;
        if (ImGui::SliderInt("Doppler I", &if_dopplerI, 0, 1))
            volume_shader.if_dopplerI_ = if_dopplerI;
        if (ImGui::SliderInt("Doppler T", &if_dopplerT, 0, 1))
            volume_shader.if_dopplerT_ = if_dopplerT;
        if (ImGui::SliderInt("Bloom_N", &bloom_N, 1, 20))
            renderer.bloom_N = bloom_N * 2;
        if (ImGui::SliderFloat("log Luminance", &LM, 1, 20))
            volume_shader.L_ = pow(10.f,LM);
        ImGui::End();
        ImGui::Render();
        renderer.enableBloom(true);
        renderer.updatePixels(volume_shader.getPowerTexture());
        //test(volume_shader.getPowerTexture());

        if (time_int % 30 == 0){
            elapsed = std::chrono::high_resolution_clock::now() - end;
            std::cout << "\r  -Compute Done " << 30 * 1000.f/elapsed.count() << "fps " << time << std::flush;
            end = std::chrono::high_resolution_clock::now();
        }
        time += elapsed.count() * 1e-6;
        time_int ++;
        //_sleep(100);
        renderer.render(false);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //renderer.render();
        glfwSwapBuffers(renderer.getWindow());
        glfwPollEvents();
    }

    return 0;
}