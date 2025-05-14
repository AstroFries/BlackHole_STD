#include <Eigen>
#include <memory>
#include <vector>
typedef std::shared_ptr<std::vector<Eigen::Vector3d>> svVector;
class RayMarch{
  public:
    RayMarch();
    void set_camera_pos(Eigen::Vector3d x0);
    void set_v(Eigen::Vector3d v0);
    virtual svVector compute_light(double step,bool stop(double,double,double)) {return nullptr;};
    svVector light;
  protected:
    Eigen::Vector3d x0,v0;
};

//Ray March in flat spacetime
class Flat_ST_RayMarch : public RayMarch {
  public:
    Flat_ST_RayMarch();
    svVector compute_light(double step,bool stop(double,double,double) = nullptr) override;
};

/*
class Schwarzschild_BH_RayMarch : public RayMarch {
  public:
    Schwarzschild_BH_RayMarch();
    double M = 0;
    svVector compute_light(double step,bool stop(double,double,double) = nullptr) override;
}*/