#ifndef SCHWARZSCHILD_MARCH_H
#define SCHWARZSCHILD_MARCH_H

#include "ray_march.h"

class Gauge_March : public RayMarch {
  public:
    Gauge_March();
    //double M = 0;
    //void set_M(double M){this->M = M;}
    svVector compute_light(double step,bool stop(double,double,double) = nullptr) override;
    //sv compute_l() override;
  private:
    sv l_;
};

#endif