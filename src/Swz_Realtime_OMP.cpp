#include "OpenGLRenderer.h"
#include "ray_march.h"
#include "Schwarzschild_march.h"
#include "OpenGLVolumeRender.h"
#include "gauge_march.h"
#include "bloom.h"
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
#include <omp.h>

#define X_SIGHT 2.0*0.6
#define Y_SIGHT 1.5*0.6
#define PATH_N 32
#define N 3
#define L 2
#define M 2
#define R 0.15
typedef std::shared_ptr<std::vector<Eigen::Vector3d>> svVector;

bool stop(double x, double y, double z){
    return x*x + y*y + z*z > 8*8;
}
int main() {
    //std::unique_ptr<RayMarch> RM0 = std::make_unique<Schwarzschild_BH_RayMarch>();
    //std::unique_ptr<RayMarch> RM0 = std::make_unique<Flat_ST_RayMarch>();
    
    //std::unique_ptr<RayMarch> RM0 = std::make_unique<Gauge_March>();
    //RM0->M_ = 0.2;
    
    double theta0 = 0.2*3.142, phi0 =  -0.305 * 1.571, r0 = 4.7;//摄像机位�??
    double theta1 = 0.2*3.142, phi1 = -0.305 * 1.571;//摄像机视�??
    const int width = 400*1;
    const int height = 300*1;
    std::cout << "Window:  " << width << " x " << height << std::endl;
    Eigen::Vector3d x0(-r0*cos(theta0)*cos(phi0),-r0*sin(theta0)*cos(phi0),-r0*sin(phi0));
    Eigen::Vector3d v1(cos(theta1)*cos(phi1),sin(theta1)*cos(phi1),sin(phi1));
    Eigen::Vector3d vx(sin(theta1),-cos(theta1),0);
    Eigen::Vector3d vy(-cos(theta1)*sin(phi1),-sin(theta1)*sin(phi1),cos(phi1));
    Eigen::Vector3d v0 = v1;
    //RM0->set_camera_pos(x0);
    //RM0->set_v(v0);
    //svVector light0 = RM0->compute_light(0.05,stop);
    std::vector<std::vector<Eigen::Vector4f>> lights(width * height);
    OpenGLRenderer renderer(width, height, "OpenGL Renderer");
    renderer.initShaders_Bloom(SHADERS_DIR);
    VolumeComputeShader shader(width, height, PATH_N);//64为体渲染采样�??
    shader.init(SHADERS_DIR "/volume_compute.comp");
    
    //ImGui Init
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(renderer.getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    //*
    std::vector<unsigned char> pixels(width * height * 3, 0);
    std::vector<float> pixels_d(width * height * 3, 0);
    for (int i = 0; i < width * height * 3; i += 3) {
        pixels[i] = 235;     // R
        pixels[i + 1] = 235;   // G
        pixels[i + 2] = 235;   // B
    }

    //------------------------------------
    //Ray Marching
    int k = 0;
    std::cout << "Ray Marching Started." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    //--------------OpenMP并行
    std::atomic<bool> should_stop(false);
    std::atomic<int> progress_counter(0);
    int total_pixels = width * height;
    int report_interval = 1000;
    #pragma omp parallel for schedule(static,1000)
    for (int k = 0; k < width * height; ++k) {
        if (glfwWindowShouldClose(renderer.getWindow())) {
            should_stop.store(true);
            continue;
        }

        // 计算相机方向
        double kx = (((double)(k % width)) / width - 0.5) / 0.5 * X_SIGHT;
        double ky = (((double)(k / width)) / height - 0.5) / 0.5 * Y_SIGHT;

        Eigen::Vector3d v0 = v1 + kx * vx + ky * vy;

        // 每个线程创建自己的 RayMarch 实例
        thread_local auto RM_thread = std::make_unique<Gauge_March>();
        //std::make_unique<Gauge_March>();
        //std::make_unique<Schwarzschild_BH_RayMarch>()
        RM_thread->M_ = 0.2;
        RM_thread->set_camera_pos(x0);
        RM_thread->set_v(v0);

        // 执行光线追踪
        svVector light0 = RM_thread->compute_light(0.05, stop);
        auto l0 = RM_thread->compute_l();

        // 获取当前像素路径存储位置
        std::vector<Eigen::Vector4f>& path = lights[k];

        // 填充路径点数据
        for (int i = 0; i < light0->size(); ++i) {
            if ((*light0)[i][2] * (*light0)[i][2] > 0.1 * 0.1)
                continue;
            if ((*light0)[i][0] * (*light0)[i][0] + (*light0)[i][1] * (*light0)[i][1] > 4 * 4)
                continue;

            Eigen::Vector4f point_i;
            point_i << (*light0)[i].x(), (*light0)[i].y(), (*light0)[i].z(), (*l0)[i];
            path.push_back(point_i);
        }

        // 如果路径过长，进行降采样
        if (path.size() > PATH_N) {
            int k_delete = path.size() / PATH_N + 1;
            for (int j = path.size() - 1; j >= 0; --j) {
                if (j % k_delete != 0)
                    path.erase(path.begin() + j);
                else
                    path[j][3] *= k_delete;
            }
        }

        // 补足路径长度
        while (path.size() < PATH_N)
            path.push_back(Eigen::Vector4f(0, 0, 0, 0));

        // 设置预览颜色（基于光子数量）

        int current = ++progress_counter;

        
        if (current % 1000 == 0) { // 至少间隔 100ms 才刷新一次
            std::cerr << "\r  loading... " << current << "/" << total_pixels << std::flush;
        }
    }

    // 并行部分结束，更新图像
    

    //----------------并行结束
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Ray Marching spent:" << elapsed.count() << "ms" <<std::endl;

    //----------------------------------
    //Volume Rendering
    std::cout << "Volume Rendering Started." << std::endl;
    start = std::chrono::high_resolution_clock::now();
    std::vector<float> Power(width * height);

    // 2. 上传路径数据
    shader.uploadPaths(lights);

    //shader.downloadPower(Power);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "  -Upload Done " << elapsed.count() << "ms" <<std::endl;
    // 3. 启动计算

    float time = 0;
    int time_int = 0;
    //shader.dispatch();
    while (!renderer.shouldClose()) {
        
        shader.dispatch();
        
        shader.time_ = time;
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Control");
        static float strength = 0.2f, LM = 6.f;
        if (ImGui::SliderFloat("Bloom Strength", &strength, 0.0f, 2.0f))
            renderer.setBloomStrength(strength);
        static float T = 5000.f;
        if (ImGui::SliderFloat("Tempetarue", &T, 500.0f, 20000.0f))
            shader.T_ = T;
        static int if_dopplerI = true, if_dopplerT = true, bloom_N = 8;
        if (ImGui::SliderInt("Doppler I", &if_dopplerI, 0, 1))
            shader.if_dopplerI_ = if_dopplerI;
        if (ImGui::SliderInt("Doppler T", &if_dopplerT, 0, 1))
            shader.if_dopplerT_ = if_dopplerT;
        if (ImGui::SliderInt("Bloom_N", &bloom_N, 1, 20))
            renderer.bloom_N = bloom_N * 2;
        if (ImGui::SliderFloat("log Luminance", &LM, 1, 20))
            shader.L_ = pow(10.f,LM);
        ImGui::End();
        ImGui::Render();
        renderer.enableBloom(true);
        renderer.updatePixels(shader.getPowerTexture());
        

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