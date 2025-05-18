#include "Schwarzschild_march.h"
#include "geodesic.h"
#include <iostream>


Schwarzschild_BH_RayMarch::Schwarzschild_BH_RayMarch(){
    std::cout << "Ray Marching method: Schwarzschild Spacetime" <<std::endl;
}

svVector Schwarzschild_BH_RayMarch::compute_light(double step,bool stop(double,double,double)){
    double r_phi = x0.norm() * (v0.dot(x0))/(v0.cross(x0).norm()+0.01);
    r_phi *= sqrt(1-2*M_/x0.norm());
    Eigen::Vector3d xhat_0 = x0.normalized();
    Eigen::Vector3d n = (x0.cross(v0)).normalized();
    Eigen::Vector3d phihat_0 = n.cross(x0).normalized();
    sv r(new std::vector<double>), phi(new std::vector<double>);
    Geodesic Geodesic0(M_);
    Geodesic0.reset(r, phi);
    Geodesic0.compute(0, x0.norm(), r_phi, step);
    l_ = Geodesic0.compute_l();
    light->clear();
    for (int i = 0; i < r->size(); ++i){
        light->push_back((*r)[i] * cos((*phi)[i]) * xhat_0 + (*r)[i] * sin((*phi)[i]) * phihat_0);
    }
    return light;
}

sv Schwarzschild_BH_RayMarch::compute_l(){
    return l_;
}