#include "ray_march.h"
#include <iostream>
typedef std::shared_ptr<std::vector<Eigen::Vector3d>> svVector;

RayMarch::RayMarch(){
    std::cout << "A Ray Marching Class has been built." << std::endl;
    light = std::make_shared<std::vector<Eigen::Vector3d>>();
}
void RayMarch::set_camera_pos(Eigen::Vector3d x0){
    this->x0 = x0;
}
void RayMarch::set_v(Eigen::Vector3d v0){
    this->v0 = v0;
}
Flat_ST_RayMarch::Flat_ST_RayMarch(){
    std::cout << "Ray Marching method: Flat Spacetime" <<std::endl;
}

svVector Flat_ST_RayMarch::compute_light(double step,bool stop(double,double,double)){
    Eigen::Vector3d v1, x = x0;
    v1 = v0 * step / v0.norm();
    light->clear();
    if (stop == nullptr){
        while (x.norm() < 10){
            light->push_back(x);
            x += v1;
        }
    }else{
        while (!stop(x[0], x[1], x[2])){
            light->push_back(x);
            x += v1;
        }
    }
    return light;
}