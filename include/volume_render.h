#include <memory>
#include <vector>
#include <Eigen>
typedef std::shared_ptr<std::vector<double>> sv;
typedef std::shared_ptr<std::vector<Eigen::Vector3d>> svVector;
class VolumeRender{
  public:
    VolumeRender(sv x, sv y, sv z);
    VolumeRender(svVector x);
    void reset(sv x, sv y, sv z);
    void reset(svVector x);
    double compute_p(double ABS(double,double,double),
                     double EMS(double,double,double));
  private:
    sv x_, y_, z_;
    svVector x_Vector_;
    std::vector<double> l_;
    bool mode = 0; // 0: sv; 1: sVector
};