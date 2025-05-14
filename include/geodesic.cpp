#include "geodesic.h"
#include <iostream>

typedef std::shared_ptr<std::vector<double>> sv;
Geodesic::Geodesic(double M){
    this->M_ = M;
    //std::cout << 1 << std::endl;
}

void Geodesic::reset(sv r, sv phi){
    this->r_ = r;
    this->phi_ = phi;
}

void Geodesic::compute(double phi_0, double r0, double r_phi, double delta){
    r_->clear();
    phi_->clear();
    mu_ = 1 / r0;
    mu_phi_ = - r_phi / r0 / r0;
    phi_->push_back(phi_0);
    while (true){
        r_->push_back(1 / mu_);
        mu_phi_ += delta * (-mu_ + 3 * M_ * mu_ * mu_);
        mu_ += delta * mu_phi_;
        if (mu_<0 || 2 * M_ * mu_ > 1)break; 
        phi_->push_back(phi_->back() + delta);
    }
}

sv Geodesic::to_x(double r_max){
    sv x(new std::vector<double>);
    if (r_ == NULL) return r_;
    for (int i = 0; i < r_->size(); ++i){
        x->push_back((*r_)[i] * cos((*phi_)[i]));
        if ((*r_)[i] > r_max) break;
    }
    return x;
}

sv Geodesic::to_y(double r_max){
    sv x(new std::vector<double>);
    if (r_ == NULL) return r_;
    for (int i = 0; i < r_->size(); ++i){
        x->push_back((*r_)[i] * sin((*phi_)[i]));
        if ((*r_)[i] > r_max) break;
    }
    return x;
}