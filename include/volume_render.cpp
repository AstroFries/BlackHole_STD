#include "volume_render.h"
#include <cmath>
typedef std::shared_ptr<std::vector<double>> sv;

VolumeRender::VolumeRender(sv x, sv y, sv z){
    reset(x,y,z);
}
VolumeRender::VolumeRender(svVector x){
    reset(x);
}
void VolumeRender::reset(sv x, sv y, sv z){
    this->x_ = x;
    this->y_ = y;
    this->z_ = z;
    l_.clear();
    for (int i = 0; i < x_->size() - 1; ++i){
        l_.push_back(
            sqrt(
                pow((*x_)[i+1]-(*x_)[i],2) + pow((*y_)[i+1]-(*y_)[i],2) + pow((*z_)[i+1]-(*z_)[i],2)
            )
        );
    }
    mode = 0;
}
void VolumeRender::reset(svVector x){
    this->x_Vector_ = x;
    l_.clear();
    for (int i = 0; i < x_Vector_->size() - 1; ++i){
        l_.push_back(
            ((*x_Vector_)[i+1] - (*x_Vector_)[i]).norm()
        );
    }
    mode = 1;
}
double VolumeRender::compute_p(double ABS(double,double,double), double EMS(double, double, double)){
    double L = 0, S = 1;
    if (mode == 0){
        for (int i = 0; i < x_->size() - 1; ++i){
            L += ABS((*x_)[i],(*y_)[i],(*z_)[i]) * l_[i] * S;
            S *= pow(2.71828, -EMS((*x_)[i],(*y_)[i],(*z_)[i]) * l_[i]);
        }
    }else {
        for (int i = 0; i < x_Vector_->size() - 1; ++i){
            L += ABS((*x_Vector_)[i][0],(*x_Vector_)[i][1],(*x_Vector_)[i][2]) * l_[i] * S;
            //S *= pow(2.71828, -EMS((*x_Vector_)[i][0],(*x_Vector_)[i][1],(*x_Vector_)[i][2]) * l_[i]);
            S *= 1 - EMS((*x_Vector_)[i][0],(*x_Vector_)[i][1],(*x_Vector_)[i][2]) * l_[i];
            if (S < 0.01)break;
        }
    }
    return L;
}