#include "OpenGLRenderer.h"
#include "ray_march.h"
#include "Schwarzschild_march.h"
#include "OpenGLVolumeRender.h"
#include "bloom.h"
#include <Eigen>
#include <vector>
#include <memory>
#include <assert.h>
#include <chrono>
#include <cmath>
#include <iostream>
#include <algorithm>

#define X_SIGHT 2.0
#define Y_SIGHT 1.5
#define N 3
#define L 2
#define M 2
#define R 0.15
typedef std::shared_ptr<std::vector<Eigen::Vector3d>> svVector;

double AccretionDisc(double x, double y, double z){
    if (x*x + y*y < 0.98*0.98 || z * z > 0.1*0.1)return 0;
    return 0.5*pow(2.72,-450*z*z-(x*x+y*y-2*sqrt(x*x+y*y)))*(pow(x*x + y*y - 0.98*0.98,1));//*(1.2+std::sin(60/pow(x*x+y*y-0.2*0.2,0.3)));//*(1.5+std::sin(40/pow(x*x+y*y-0.2*0.2,0.3)-52*x*x-52*y*y));
}
double ABS(double x, double y, double z, double vx, double vy, double vz){
    if (x*x + y*y < 0.98*0.98 || z * z > 0.1*0.1)return 0;
    double M_ = 0.2;
    double r_ = sqrt(x * x + y * y);
    double dv = (vx * x + vy * y + vz * z)/(x * x + y * y + z * z) * (1/sqrt(1-2*M_/r_) - 1);
    vx += dv * x;
    vy += dv * y;
    vz += dv * z;
    double cos_theta = (x * vy - y* vx)/r_/sqrt(vx*vx + vy*vy + vz*vz);
    double gamma2 = (r_ - 2 * M_)/M_;
    double beta = sqrt(M_/(r_ - 2 * M_));
    return 1.35*AccretionDisc(x, y, z)/gamma2 / gamma2 / pow(1+ beta * cos_theta,4);//*vz;
    
}
double EMS(double x, double y, double z){
    return 2.02*AccretionDisc(x, y, z);
}
bool stop(double x, double y, double z){
    return x*x + y*y + z*z > 8*8;
}
int main() {
    std::unique_ptr<RayMarch> RM0 = std::make_unique<Schwarzschild_BH_RayMarch>();
    //std::unique_ptr<RayMarch> RM0 = std::make_unique<Flat_ST_RayMarch>();
    RM0->M_ = 0.2;
    
    double theta0 = 0.2*3.142, phi0 =  -0.12 * 1.571, r0 = 3.7;
    double theta1 = 0.30*3.142, phi1 = -0.22 * 1.571;
    const int width = 400*4;
    const int height = 300*4;
    std::cout << "Window:  " << width << " x " << height << std::endl;
    Eigen::Vector3d x0(-r0*cos(theta0)*cos(phi0),-r0*sin(theta0)*cos(phi0),-r0*sin(phi0));
    Eigen::Vector3d v1(cos(theta1)*cos(phi1),sin(theta1)*cos(phi1),sin(phi1));
    Eigen::Vector3d vx(sin(theta1),-cos(theta1),0);
    Eigen::Vector3d vy(-cos(theta1)*sin(phi1),-sin(theta1)*sin(phi1),cos(phi1));
    Eigen::Vector3d v0 = v1;
    RM0->set_camera_pos(x0);
    RM0->set_v(v0);
    svVector light0 = RM0->compute_light(0.005,stop);
    std::vector<std::vector<Eigen::Vector4f>> lights(width * height);
    OpenGLRenderer renderer(width, height, "OpenGL Renderer");
    VolumeComputeShader shader(width, height, 64);
    shader.init(SHADERS_DIR "/volume_compute.comp");
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
    float max_p = 0;
    std::cout << "Ray Marching Started." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    while (!renderer.shouldClose() && k < width * height) {
        double kx,ky;
        kx = (((double)(k%width))/width - 0.5)/0.5 * X_SIGHT;
        ky = (((double)(k/width))/height - 0.5)/0.5 * Y_SIGHT;            
        //Warning : the v0 is no the real v0 in the Schwarzschild Spacetime,because of the 
        //          g_rr is sqrt(1-2M/r)^{-1} instead of 1 which in R^3,the method is let the r_phi *= sqrt(1-2M/r)
        v0 = v1 + kx * vx + ky * vy;
        //std::cout << (v2-v1) << std::endl;
        RM0->set_v(v0);
        light0 = RM0->compute_light(0.009,stop);
        auto l0 = RM0->compute_l();
        for (int i = 0; i < light0->size(); ++i){
            if(pow((*light0)[i][2],2) > 0.1 * 0.1)continue;
            Eigen::Vector4f point_i;
            for (int j = 0; j < 3;j++)point_i[j] = (*light0)[i][j];
            point_i[3] = (*l0)[i];
            lights[k].push_back(point_i);
            //if (lights[k].size() == 64)break;
        }
        if (lights[k].size() > 64){
            int k_delete = (lights[k].size()/64) + 1;
            for (int j = lights[k].size() - 1; j >=0; j--){
                if (j % k_delete != 0){
                    lights[k].erase(lights[k].begin() + j);
                }else {
                    lights[k][j][3] *= k_delete;
                }
            }
        }
        while (lights[k].size() < 64) {
            lights[k].push_back(Eigen::Vector4f(0, 0, 0, 0));
        }
        //if (lights[k].size() >64)std::cout << lights[k].size() << std::endl;
        pixels[k * 3 ] = std::min((int)(*light0).size(),255);
        pixels[k * 3 +1] = std::min((int)(*light0).size(),255);
        pixels[k * 3 +2] = std::min((int)(*light0).size(),255);
        k++; 
        if (k % width == 1 ){
            renderer.updatePixels(pixels);
            renderer.render();
        }
    }//*/
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Ray Marching spent:" << elapsed.count() << "ms" <<std::endl;

    //----------------------------------
    //Volume Rendering
    k = 0;
    std::cout << "Volume Rendering Started." << std::endl;
    start = std::chrono::high_resolution_clock::now();
    std::vector<float> Power(width * height);

    // 2. 上传路径数据（lights 必须是 std::vector<std::vector<Eigen::Vector4f>>，每像素最多64条路径）
    shader.uploadPaths(lights);

    //shader.downloadPower(Power);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "  -Upload Done " << elapsed.count() << "ms" <<std::endl;
    // 3. 启动计算
    shader.dispatch();
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "  -Compute Done " << elapsed.count() << "ms" <<std::endl;
    // 4. 读取结果
    shader.downloadPower(pixels_d);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "  -Download Done " << elapsed.count() << "ms" <<std::endl;
    
    while (!renderer.shouldClose() && k < width * height) {
        //std::cout << Power << std::endl;
        //if (Power[k] > 0)std::cout << k%width << ' ' << k/width << ' ' << Power[k] << std::endl;
        //if (Power > 0.01)std::cout << (int)(atan(100.0 * Power) / 1.571 * 256.0) << std::endl;
        //pixels_d[3 * k] = ((5 * Power[k])  * 255.0);
        //pixels_d[3 * k + 1] = ((5 * Power[k])  * 127.0);
        //pixels_d[3 * k + 2] = ((5 * Power[k])  * 74.0);
        for (int j = 0; j < 3; ++j)pixels_d[3 * k +j] = pow(pixels_d[3 * k +j],1/2.2);
        for (int j = 0; j < 3; ++j)max_p = (pixels_d[3 * k + j] > max_p)?pixels_d[3 * k + j] : max_p;
        //std::cout << k << ' ' << Power[k] << std::endl;
        pixels[k * 3 ] = 127;
        pixels[k * 3 + 1] = 127;
        pixels[k * 3 + 2] = 127;
        k++;
        
    }//*/
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Volume Rendering spent:" << elapsed.count() << "ms" <<std::endl;
    glClear(GL_COLOR_BUFFER_BIT);
    renderer.updatePixels(pixels);
    renderer.render();
    
    //------------------------------------
    //Color Reset
    std::cout << "Color Reset Started." << std::endl;
    std::cout << max_p << std::endl;
    for (int i = 0; i < width * height ; ++i){
        for (int j = 0; j < 3; ++j)pixels_d[3 * i + j] *= 1.0 * 255/max_p;//2*pixels_d[3 * i] / max_p /max_p;
        //if (pixels_d[3*i]>1 || pixels_d[3*i] < 0)std::cout << k << std::endl;
        //std::cout << i << ' ' <<(int)(1.0*pixels_d[3 * i] *254.0 ) << std::endl;
        pixels[i * 3 + 0] = std::min((int)(2.5*pixels_d[3 * i]  ),255);
        pixels[i * 3 + 1] = std::min((int)(2.5*pixels_d[3 * i + 1]  ),255);
        pixels[i * 3 + 2] = std::min((int)(2.5*pixels_d[3 * i + 2]  ),255);
        //pixels_d[3 * i] *= 1.4*pixels_d[3 * i] *254.0;
        //pixels_d[3 * i + 1] = 1.0*pixels_d[3 * i] *127.0;
        //pixels_d[3 * i + 2] = 1.0*pixels_d[3 * i] *74.0;
        //pixels_d[3 * i] = 1.0*pixels_d[3 * i] *254.0;
    }

    //------------------------------------
    //Blooming
    renderer.updatePixels(pixels);
    renderer.render();
    std::cout << "Blooming1" <<std::endl;
    Bloom bloomer(width, height, &pixels_d, &pixels);
    bloomer.bloom_gauss(12, 0.5);
    renderer.updatePixels(pixels);
    renderer.render();
    std::cout << "Blooming2" <<std::endl;
    bloomer.bloom_gauss(150, 0.25);//*/
    while (!renderer.shouldClose()) {
        renderer.updatePixels(pixels);
        renderer.render();
        
    }//*/
    return 0;
}