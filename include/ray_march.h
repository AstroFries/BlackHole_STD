#ifndef RAY_MARCH_H
#define RAY_MARCH_H

#include <Eigen>
#include <memory>
#include <vector>
typedef std::shared_ptr<std::vector<Eigen::Vector3d>> svVector;
typedef std::shared_ptr<std::vector<double>> sv;
class RayMarch{
  public:
    RayMarch();
    void set_camera_pos(Eigen::Vector3d x0);
    void set_v(Eigen::Vector3d v0);
    virtual svVector compute_light(double step,bool stop(double,double,double)) {return nullptr;};
    virtual sv compute_l(){return nullptr;};
    svVector light;
    double M_;
  protected:
    Eigen::Vector3d x0,v0;
};

//Ray March in flat spacetime
class Flat_ST_RayMarch : public RayMarch {
  public:
    Flat_ST_RayMarch();
    svVector compute_light(double step,bool stop(double,double,double) = nullptr) override;
    sv compute_l() override;
  private:
    sv l_;
};

#endif