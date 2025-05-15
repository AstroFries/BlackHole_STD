#ifndef SCHWARZSCHILD_MARCH_H
#define SCHWARZSCHILD_MARCH_H

#include "ray_march.h"

class Schwarzschild_BH_RayMarch : public RayMarch {
  public:
    Schwarzschild_BH_RayMarch();
    //double M = 0;
    //void set_M(double M){this->M = M;}
    svVector compute_light(double step,bool stop(double,double,double) = nullptr) override;
};

#endif