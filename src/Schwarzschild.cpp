#include "OpenGLRenderer.h"
#include "ray_march.h"
#include "volume_render.h"
#include "Schwarzschild_march.h"
#include <Eigen>
#include <vector>
#include <memory>
#include <cmath>
#include <iostream>

#define X_SIGHT 2.0
#define Y_SIGHT 1.5
#define N 3
#define L 2
#define M 2
#define R 0.15
typedef std::shared_ptr<std::vector<Eigen::Vector3d>> svVector;

double AccretionDisc(double x, double y, double z){
    if (x*x + y*y < 1*1)return 0;
    return 0.5*pow(2.72,-15*z*z-(x*x+y*y-2*sqrt(x*x+y*y)))*(pow(x*x + y*y - 1,0.5));
}
double ABS(double x, double y, double z){
    return 0.2*AccretionDisc(x, y, z);
}
double EMS(double x, double y, double z){
    return 2.52*AccretionDisc(x, y, z);
}
bool stop(double x, double y, double z){
    return x*x + y*y + z*z > 5*5;
}
int main() {
    //std::unique_ptr<VolumeRender> VR0= std::make_unique<VolumeRender>();
    std::unique_ptr<RayMarch> RM0 = std::make_unique<Schwarzschild_BH_RayMarch>();
    //std::unique_ptr<RayMarch> RM0 = std::make_unique<Flat_ST_RayMarch>();
    RM0->M_ = 0.2;
    //double theta0 = 
    double theta0 = 0.2*3.142, phi0 = -0.15 * 1.571, r0 = 3.3;
    double theta1 = 0.35*3.142, phi1 = -0.25 * 1.571;
    Eigen::Vector3d x0(-r0*cos(theta0)*cos(phi0),-r0*sin(theta0)*cos(phi0),-r0*sin(phi0));
    Eigen::Vector3d v1(cos(theta1)*cos(phi1),sin(theta1)*cos(phi1),sin(phi1));
    Eigen::Vector3d vx(sin(theta1),-cos(theta1),0);
    Eigen::Vector3d vy(-cos(theta1)*sin(phi1),-sin(theta1)*sin(phi1),cos(phi1));
    Eigen::Vector3d v0 = v1;
    RM0->set_camera_pos(x0);
    RM0->set_v(v0);
    svVector light0 = RM0->compute_light(0.002,stop);
    //for (int i = 0; i < light0->size(); ++i) std::cout << (*light0)[i][0] << std::endl;
    std::unique_ptr<VolumeRender> VR0= std::make_unique<VolumeRender>(light0);
    //std::cout << VR0->compute_p(ABS,EMS) << std::endl;
    
    const int width = 400*4;
    const int height = 300*4;

    OpenGLRenderer renderer(width, height, "OpenGL Renderer");
    //*
    std::vector<unsigned char> pixels(width * height * 3, 0);
    std::vector<float> pixels_d(width * height, 0);
    for (int i = 0; i < width * height * 3; i += 3) {
        pixels[i] = 235;     // R
        pixels[i + 1] = 235;   // G
        pixels[i + 2] = 235;   // B
    }
    int k = 0;
    float max_p = 0;
    while (!renderer.shouldClose()) {
        if (k < width * height ){
            double kx,ky;
            kx = (((double)(k%width))/width - 0.5)/0.5 * X_SIGHT;
            ky = (((double)(k/width))/height - 0.5)/0.5 * Y_SIGHT;
            //v0 = Eigen::Vector3d( 1,-kx,ky);
            
            v0 = v1 + kx * vx + ky * vy;
            //std::cout << (v2-v1) << std::endl;
            RM0->set_v(v0);
            light0 = RM0->compute_light(0.005,stop);
            VR0->reset(light0);
            double Power = VR0->compute_p(ABS, EMS);
            //std::cout << Power << std::endl;
            //if (Power > 0.01)std::cout << (int)(atan(100.0 * Power) / 1.571 * 256.0) << std::endl;
            pixels[k * 3 ] = 0;
            //pixels[k * 3 + 1] = (int)(atan(5 * Power) / 1.571 * 255.0); 
            //pixels[k * 3 + 2] = (int)(atan(3 * Power) / 1.571 * 255.0);
            pixels_d[k] = ((5 * Power)  * 255.0);
            max_p = (pixels_d[k] > max_p)?pixels_d[k] : max_p;
            k++; 
        } else if (k == width * height){
            for (int i = 0; i < width * height - 1; ++i){
                //if (i%width == 0)std::cout << i/width << std::endl;
                pixels[i * 3 + 0] = (int)(pixels_d[i] *254.0 / max_p);
                pixels[i * 3 + 1] = (int)(pixels_d[i] *247.0 / max_p);
                pixels[i * 3 + 2] = (int)(pixels_d[i] *204.0 / max_p);
            }
            k++;
        }
        //std::cout << k++ << std::endl;
        if (k % width == 1){
            renderer.updatePixels(pixels);
            renderer.render();
        }
    }//*/

    return 0;
}