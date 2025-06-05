
#include "matplotlibcpp.h"
#include "geodesic.h"
#include "gauge_march.h"
#include <vector>
#include <Eigen>
#include <memory>
#include <iostream>

namespace plt = matplotlibcpp;
typedef std::shared_ptr<std::vector<double>> sv;
int main()
{

    std::unique_ptr<RayMarch> gauge_march = std::make_unique<Gauge_March>();
    gauge_march->M_ = 1;
    for (int i = 0; i < 5; ++i){
        //for (int j = 0; j < 2; ++j){
        Eigen::Vector3d x0(2.9 + i * 0.2,0,0), v0(-1.0,0.0000,1);
        double step = 0.01;
        gauge_march->set_camera_pos(x0);
        gauge_march->set_v(v0);
        auto light = gauge_march->compute_light(step,nullptr);
        std::vector<double> x,y,z;
        x.push_back(0);
        y.push_back(0);
        for (int i = 0; i < light->size(); ++i){
            x.push_back((*light)[i][0]);
            y.push_back((*light)[i][2]);
            //std::cout << x.back() << ' ' << y.back() << std::endl;
        }
        plt::plot(x,y);
        //}
    }
    /*
    for (int i = 0; i < 5; ++i){
        BH.compute(0, 3.1 + i * 0.2, 0, 0.01);
        plt::plot(*(BH.to_x(10)),*(BH.to_y(10)));
    }*/
    PyRun_SimpleString("import matplotlib.pyplot as plt");
    PyRun_SimpleString("plt.gca().set_aspect('equal', adjustable='box')");

    plt::show();
    
    return 0;
}