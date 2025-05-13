#include <vector>
#include <memory>

typedef std::shared_ptr<std::vector<double>> sv;

class Geodesic{
  public:
    Geodesic(double M);
    void reset(sv r, sv phi);
    void compute(double phi_0, double r0, double r_phi, double delta);
    sv to_x(double r_max = 20);
    sv to_y(double r_max = 20);
  private:
    sv r_,phi_;
    double mu_phi_, mu_;
    double M_;
};