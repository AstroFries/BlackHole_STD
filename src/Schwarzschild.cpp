#include "OpenGLRenderer.h"
#include "ray_march.h"
#include "volume_render.h"
#include "Schwarzschild_march.h"
#include "bloom.h"
#include <Eigen>
#include <vector>
#include <memory>
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
    if (x*x + y*y < 0.3*0.3 || z * z > 0.1*0.1)return 0;
    return 0.5*pow(2.72,-450*z*z-(x*x+y*y-2*sqrt(x*x+y*y)))*(pow(x*x + y*y - 0.3*0.3,1));//*(1.2+std::sin(60/pow(x*x+y*y-0.2*0.2,0.3)));//*(1.5+std::sin(40/pow(x*x+y*y-0.2*0.2,0.3)-52*x*x-52*y*y));
}
double ABS(double x, double y, double z){
    return 1.35*AccretionDisc(x, y, z);
}
double EMS(double x, double y, double z){
    return 2.02*AccretionDisc(x, y, z);
}
bool stop(double x, double y, double z){
    return x*x + y*y + z*z > 5*5;
}
int main() {
    //setup the Ray Marching Class : Using Schwarzschild Black Hole Rule
    std::unique_ptr<RayMarch> RM0 = std::make_unique<Schwarzschild_BH_RayMarch>();
    //std::unique_ptr<RayMarch> RM0 = std::make_unique<Flat_ST_RayMarch>();
    RM0->M_ = 0.2;
    
    //the position of the camrea
    double theta0 = 0.2*3.142, phi0 = -0.15 * 1.571, r0 = 3.7;
    //the position of the sight
    double theta1 = 0.28*3.142, phi1 = -0.25 * 1.571;

    //several direction
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
    
    const int width = 400*3;
    const int height = 300*3;

    OpenGLRenderer renderer(width, height, "OpenGL Renderer");
    //*
    std::vector<unsigned char> pixels(width * height * 3, 0);
    std::vector<float> pixels_d(width * height * 3, 0);
    //useless color init,was used for testing 
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
            
            //Warning : the v0 is no the real v0 in the Schwarzschild Spacetime,because of the 
            //          g_rr is sqrt(1-2M/r)^{-1} instead of 1 which in R^3,the method is let the r_phi *= sqrt(1-2M/r)
            v0 = v1 + kx * vx + ky * vy;
            //std::cout << (v2-v1) << std::endl;
            RM0->set_v(v0);
            light0 = RM0->compute_light(0.05,stop);
            VR0->reset(light0);
            VR0->set_l(RM0->compute_l());
            double Power = VR0->compute_p(ABS, EMS);
            //std::cout << Power << std::endl;
            //if (Power > 0.01)std::cout << (int)(atan(100.0 * Power) / 1.571 * 256.0) << std::endl;
            pixels[k * 3 ] = 0;
            pixels[k * 3 +1] = 0;
            pixels[k * 3 +2] = 0;
            pixels_d[3 * k] = ((5 * Power)  * 255.0);
            pixels_d[3 * k + 1] = ((5 * Power)  * 127.0);
            pixels_d[3 * k + 2] = ((5 * Power)  * 74.0);
            max_p = (pixels_d[3 * k] > max_p)?pixels_d[3 * k] : max_p;
            k++; 
        } else if (k == width * height){
            for (int i = 0; i < width * height - 1; ++i){
                //if (i%width == 0)std::cout << i/width << std::endl;
                //pixels_d[i] = std::min(1.0*pixels_d[i],254.0);
                pixels_d[3 * i] *= 2/max_p;//2*pixels_d[3 * i] / max_p /max_p;
                pixels[i * 3 + 0] = std::min((int)(1.4*pixels_d[3 * i] *254.0 ),255);
                pixels[i * 3 + 1] = std::min((int)(1.4*pixels_d[3 * i] *127.0 ),255);
                pixels[i * 3 + 2] = std::min((int)(1.4*pixels_d[3 * i] *74.0 ),255);
                //pixels_d[3 * i] *= 1.4*pixels_d[3 * i] *254.0;
                pixels_d[3 * i + 1] = 1.0*pixels_d[3 * i] *127.0;
                pixels_d[3 * i + 2] = 1.0*pixels_d[3 * i] *74.0;
                pixels_d[3 * i] = 1.0*pixels_d[3 * i] *254.0;
            }
            break;
            k++;
        }
        //std::cout << k++ << std::endl;
        if (k % width == 1 ){
            renderer.updatePixels(pixels);
            renderer.render();
        }
    }//*/
    renderer.updatePixels(pixels);
    renderer.render();
    std::cout << "Blooming1" <<std::endl;
    Bloom bloomer(width, height, &pixels_d, &pixels);
    bloomer.bloom_gauss(12, 0.2);
    renderer.updatePixels(pixels);
    renderer.render();
    std::cout << "Blooming2" <<std::endl;
    bloomer.bloom_gauss(150, 0.25);
    while (!renderer.shouldClose()) {
        renderer.updatePixels(pixels);
        renderer.render();
        
    }//*/
    return 0;
}