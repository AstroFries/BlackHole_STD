#include "OpenGLRenderer.h"
#include "ray_march.h"
#include "volume_render.h"
#include <Eigen>
#include <vector>
#include <memory>
#include <cmath>
#include <iostream>
#include <gsl/gsl_sf_laguerre.h>
#include <gsl/gsl_sf_legendre.h>
#include <gsl/gsl_sf_gamma.h>

#define X_SIGHT 2.0
#define Y_SIGHT 1.5
#define N 3
#define L 2
#define M 2
#define R 0.15
typedef std::shared_ptr<std::vector<Eigen::Vector3d>> svVector;

double hydrogen_wave(int n, int l, int m, double x, double y, double z) {
    // 1. 直角坐标转球坐标
    double r = std::sqrt(x*x + y*y + z*z);
    double theta = (r == 0) ? 0 : std::acos(z / r);
    r /= R;
    double phi = std::atan2(y, x);

    // 2. 归一化常数
    double norm = std::sqrt(
        std::pow(2.0/(double)n, 3) * gsl_sf_gamma(n-l) /
        (2.0*n * gsl_sf_gamma(n+l+1))
    );

    // 3. 径向部分
    double rho = 2.0 * r / n;
    double radial = std::exp(-rho/2) * std::pow(rho, l) *
        gsl_sf_laguerre_n(n-l-1, 2*l+1, rho);

    // 4. 角向部分（球谐函数模长，忽略e^{im\phi}的相位）
    double sph = gsl_sf_legendre_sphPlm(l, std::abs(m), std::cos(theta));
    if (m < 0) sph *= (m % 2 == 0 ? 1 : -1); // Condon-Shortley phase

    // 5. 总波函数（不含e^{im\phi}相位）
    return pow(norm * radial * sph,2);
    //return abs(norm * radial * sph);
}
double ABS_H(double x, double y, double z){
    //if (z>0) return 0;
    return 3.8*hydrogen_wave(N,L,M,x,y,z);
}
double EMS_H(double x, double y, double z){
    //if (z>0) return 0;
    return 2175.0*hydrogen_wave(N,L,M,x,y,z);
}
double ABS(double x, double y, double z){
    return 0.2*pow(2.71828, - x*x - y*y - z*z);
}
double EMS(double x, double y, double z){
    return 0.2*pow(2.71828, - x*x - y*y - z*z);
}
bool stop(double x, double y, double z){
    return x*x + y*y + z*z > 5*5;
}
int main() {
    //std::unique_ptr<VolumeRender> VR0= std::make_unique<VolumeRender>();
    std::unique_ptr<RayMarch> RM0 = std::make_unique<Flat_ST_RayMarch>();
    //double theta0 = 
    double theta0 = 0.2*3.142, phi0 = -0.4 * 1.571, r0 = 4.5;
    Eigen::Vector3d x0(-r0*cos(theta0)*cos(phi0),-r0*sin(theta0)*cos(phi0),-r0*sin(phi0));
    Eigen::Vector3d v1(cos(theta0)*cos(phi0),sin(theta0)*cos(phi0),sin(phi0));
    Eigen::Vector3d vx(sin(theta0),-cos(theta0),0);
    Eigen::Vector3d vy(-cos(theta0)*sin(phi0),-sin(theta0)*sin(phi0),cos(phi0));
    //std::cout << v1 << vx << vy << std::endl;
    Eigen::Vector3d v0 = v1;
    RM0->set_camera_pos(x0);
    RM0->set_v(v0);
    svVector light0 = RM0->compute_light(0.02,stop);
    //for (int i = 0; i < light0->size(); ++i) std::cout << (*light0)[i][0] << std::endl;
    std::unique_ptr<VolumeRender> VR0= std::make_unique<VolumeRender>(light0);
    //std::cout << VR0->compute_p(ABS,EMS) << std::endl;
    
    const int width = 800;
    const int height = 600;

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
            light0 = RM0->compute_light(0.1,stop);
            VR0->reset(light0);
            VR0->compute_l();
            double Power = VR0->compute_p(ABS_H, EMS_H);
            //std::cout << Power << std::endl;
            //if (Power > 0.01)std::cout << (int)(atan(100.0 * Power) / 1.571 * 256.0) << std::endl;
            pixels[k * 3] = 0;
            //pixels[k * 3 + 1] = (int)(atan(5 * Power) / 1.571 * 255.0); 
            //pixels[k * 3 + 2] = (int)(atan(3 * Power) / 1.571 * 255.0);
            pixels_d[k] = (atan(5 * Power) / 1.572 * 255.0);
            max_p = (pixels_d[k] > max_p)?pixels_d[k] : max_p;
            k++; 
        } else if (k == width * height){
            for (int i = 0; i < width * height - 1; ++i){
                //if (i%width == 0)std::cout << i/width << std::endl;
                pixels[i * 3 + 1] = (int)(pixels_d[i] *254.0 / max_p);
                pixels[i * 3 + 2] = (int)(pixels_d[i] *254.0*3.0 /5.0/ max_p);
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