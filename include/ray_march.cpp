#include "ray_march.h"
#include <iostream>
typedef std::shared_ptr<std::vector<Eigen::Vector3d>> svVector;

RayMarch::RayMarch(){
    //std::cout << "A Ray Marching Class has been built." << std::endl;
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
    svVector new_light = std::make_shared<std::vector<Eigen::Vector3d>>();
    sv new_l = std::make_shared<std::vector<double>>();
    l_ = new_l;
    if (stop == nullptr){
        while (x.norm() < 10){
            new_light->push_back(x);
            l_->push_back(step);
            x += v1;
        }
    }else{
        while (!stop(x[0], x[1], x[2])){
            new_light->push_back(x);
            l_->push_back(step);
            x += v1;
        }
    }
    light = new_light;
    return new_light;
}
sv Flat_ST_RayMarch::compute_l(){
    return l_;
}