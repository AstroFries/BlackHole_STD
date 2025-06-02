#include "matplotlibcpp.h"
#include "geodesic.h"
#include <vector>
#include <Eigen>
#include <memory>
#include <iostream>

namespace plt = matplotlibcpp;
typedef std::shared_ptr<std::vector<double>> sv;
int main()
{
    std::cout << 1 <<std::endl;
    Geodesic BH(1);
    sv r(new std::vector<double>), phi(new std::vector<double>);
    BH.reset(r, phi);

    for (int i = 0; i < 5; ++i){
        BH.compute(0, 3.5 + i * 0.2, 0, 0.01);
        plt::plot(*(BH.to_x(10)),*(BH.to_y(10)));
    }
    PyRun_SimpleString("import matplotlib.pyplot as plt");
    PyRun_SimpleString("plt.gca().set_aspect('equal', adjustable='box')");

    plt::show();
}