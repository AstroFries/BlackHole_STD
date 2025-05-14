#include "OpenGLRenderer.h"
#include "ray_march.h"
#include "volume_render.h"
#include <Eigen>
#include <vector>
#include <memory>
#include <cmath>
#include <iostream>
#define X_SIGHT 2.0
#define Y_SIGHT 1.5

typedef std::shared_ptr<std::vector<Eigen::Vector3d>> svVector;
double ABS(double x, double y, double z){
    return 0.2*pow(2.71828, - x*x - y*y - z*z);
}
double EMS(double x, double y, double z){
    return 0.2*pow(2.71828, - x*x - y*y - z*z);
}
bool stop(double x, double y, double z){
    return x*x + y*y + z*z > 4*4;
}
int main() {
    //std::unique_ptr<VolumeRender> VR0= std::make_unique<VolumeRender>();
    std::unique_ptr<RayMarch> RM0 = std::make_unique<Flat_ST_RayMarch>();
    Eigen::Vector3d x0(-4,0,0), v0(1,0.2,0);
    RM0->set_camera_pos(x0);
    RM0->set_v(v0);
    svVector light0 = RM0->compute_light(0.05,stop);
    //for (int i = 0; i < light0->size(); ++i) std::cout << (*light0)[i][0] << std::endl;
    std::unique_ptr<VolumeRender> VR0= std::make_unique<VolumeRender>(light0);
    std::cout << VR0->compute_p(ABS,EMS) << std::endl;
    
    const int width = 800;
    const int height = 600;

    OpenGLRenderer renderer(width, height, "OpenGL Renderer");
    //*
    std::vector<unsigned char> pixels(width * height * 3, 0);
    for (int i = 0; i < width * height * 3; i += 3) {
        pixels[i] = 255;     // R
        pixels[i + 1] = 255;   // G
        pixels[i + 2] = 255;   // B
    }
    int k = 0;
    while (!renderer.shouldClose()) {
        if (k < width * height ){
            v0 = Eigen::Vector3d( 1,
                                  -(((double)(k%width))/width - 0.5)/0.5 * X_SIGHT,
                                  (((double)(k/width))/height - 0.5)/0.5 * Y_SIGHT);
            //v0 = Eigen::Vector3d( 1,
            //                      -(((double)(k/width))/height - 0.5)/0.5 * Y_SIGHT,
            //                      (((double)(k%width))/width - 0.5)/0.5 * X_SIGHT);
                                  //std::cout << (((double)(k%width))/width - 0.5)/0.5 * X_SIGHT << std::endl;
            RM0->set_v(v0);
            light0 = RM0->compute_light(0.02,stop);
            VR0->reset(light0);
            double Power = VR0->compute_p(ABS,EMS);
            //std::cout << Power << std::endl;
            //if (Power > 0.01)std::cout << (int)(atan(100.0 * Power) / 1.571 * 256.0) << std::endl;
            pixels[k * 3] = 0;
            pixels[k * 3 + 1] = (int)(atan(5 * Power) / 1.571 * 256.0); 
            pixels[k * 3 + 2] = (int)(atan(3 * Power) / 1.571 * 256.0);  
            k++; 
        } 
        //std::cout << k++ << std::endl;
        if (k % width == 0){
            renderer.updatePixels(pixels);
            renderer.render();
        }
    }//*/

    return 0;
}