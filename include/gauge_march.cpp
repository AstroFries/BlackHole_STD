#include "gauge_march.h"
#include "output_gauge_sph_inv.h"
#include "output_christoffel_sph.h"
#include <iostream>
#include <algorithm>
#include <cmath>

typedef double (*g_sph_func)(double, double, double, double);
constexpr g_sph_func g_sph_table[4][4] = {
    {g_sph<0>, g_sph<1>, g_sph<2>, g_sph<3>},
    {g_sph<4>, g_sph<5>, g_sph<6>, g_sph<7>},
    {g_sph<8>, g_sph<9>, g_sph<10>, g_sph<11>},
    {g_sph<12>, g_sph<13>, g_sph<14>, g_sph<15>}
};
typedef double (*gamma_sph_func)(double, double, double, double);
constexpr gamma_sph_func gamma_sph_table[4][4][4] = {
    {
        {gamma_sph<0>, gamma_sph<1>, gamma_sph<2>, gamma_sph<3>},
        {gamma_sph<4>, gamma_sph<5>, gamma_sph<6>, gamma_sph<7>},
        {gamma_sph<8>, gamma_sph<9>, gamma_sph<10>, gamma_sph<11>},
        {gamma_sph<12>, gamma_sph<13>, gamma_sph<14>, gamma_sph<15>}
    },
    {
        {gamma_sph<16>, gamma_sph<17>, gamma_sph<18>, gamma_sph<19>},
        {gamma_sph<20>, gamma_sph<21>, gamma_sph<22>, gamma_sph<23>},
        {gamma_sph<24>, gamma_sph<25>, gamma_sph<26>, gamma_sph<27>},
        {gamma_sph<28>, gamma_sph<29>, gamma_sph<30>, gamma_sph<31>}
    },
    {
        {gamma_sph<32>, gamma_sph<33>, gamma_sph<34>, gamma_sph<35>},
        {gamma_sph<36>, gamma_sph<37>, gamma_sph<38>, gamma_sph<39>},
        {gamma_sph<40>, gamma_sph<41>, gamma_sph<42>, gamma_sph<43>},
        {gamma_sph<44>, gamma_sph<45>, gamma_sph<46>, gamma_sph<47>}
    },
    {
        {gamma_sph<48>, gamma_sph<49>, gamma_sph<50>, gamma_sph<51>},
        {gamma_sph<52>, gamma_sph<53>, gamma_sph<54>, gamma_sph<55>},
        {gamma_sph<56>, gamma_sph<57>, gamma_sph<58>, gamma_sph<59>},
        {gamma_sph<60>, gamma_sph<61>, gamma_sph<62>, gamma_sph<63>}
    }
};

Gauge_March::Gauge_March(){
    //std::cout << "Ray Marching method: Default Spacetime" <<std::endl;
}

inline void xyz_to_rtp(Eigen::Vector3d x0, double* r, double* theta, double* phi){
    *r = x0.norm() ;
    *theta = std::acos(x0.z() / *r);
    *phi = std::atan2(x0.y(),x0.x());
}
svVector Gauge_March::compute_light(double step,bool stop(double,double,double)){
    double r[4] = {0,0,0,0};//r theta phi t
    x0 = x0 / M_;
    xyz_to_rtp(x0, r+0, r+1, r+2);
    
    Eigen::Vector3d v0_norm = v0.normalized();
    double v[4] = {0,0,0,0};
    xyz_to_rtp(x0 + v0 * 1e-6, v+0, v+1, v+2);
    for (int i = 0; i < 3; ++i)v[i] = (v[i] - r[i]) * 1e6;
    
    //calculate ds^2 = g_33 dt^2 + (g03+g30)dtdr + (g13+g31)dtdtheta + (g23+g32)dtdphi + \sum_{i,j<3}(g_ij dx_i dx_j)
    //wish ds^2 = 0 , so  Adt^2 + Bdt + C = 0
    
    //根据初始位置的度规由vr,vtheta,vphi解出vt (先把二次方程的系数列出来)
    //为了时间反演，vt 应该是负值
    /*
    for (int i = 0; i < 4; ++i){
        for (int j = 0; j < 4; ++j){
            std::cout << g_sph_table[i][j](r[0], r[1], r[2], r[3]) << ' ';
        }
        std::cout << std::endl;
    }*/

    double A = g_sph_table[3][3](r[0], r[1], r[2], r[3]); // g_33 (t,t)
    double B = 0.0;
    for (int i = 0; i < 3; ++i){
        B += (g_sph_table[i][3](r[0], r[1], r[2], r[3]) + g_sph_table[3][i](r[0], r[1], r[2], r[3])) * v[i];
    }
    double C = 0.0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            C += g_sph_table[i][j](r[0], r[1], r[2], r[3]) * v[i] * v[j];
        }
    }
    // 解二次方程 A vt^2 + B vt + C = 0
    if (std::abs(A) > 1e-12) {
        double delta = B * B - 4 * A * C;
        if (delta < 0) {
            std::cerr << "No real solution for vt!" << std::endl;
            v[3] = 0;
        } else {
            // 选择负根（时间反演）
            v[3] = (-B - std::sqrt(delta)) / (2 * A);
            if (v[3] > 0)v[3] = (-B + std::sqrt(delta)) / (2 * A);
        }
    } else if (std::abs(B) > 1e-12) {
        v[3] = -C / B;
    } else {
        std::cerr << "A and B are both zero, cannot solve for vt!" << std::endl;
        v[3] = 0;
    }
    //std::cout << "vt = " << v[3] << std::endl;
    //for (int i = 0; i < 3; ++i){
    //    std::cout << r[i] << ' ' << v[i] << std::endl;
    //}
    sv new_l = std::make_shared<std::vector<double>>();
    svVector new_light = std::make_shared<std::vector<Eigen::Vector3d>>();
    for (int t = 0; t < 5000 && r[3] > -60; ++t) {
        xyz_to_rtp(x0, r+0, r+1, r+2);
        xyz_to_rtp(x0 + v0 * 1e-6, v+0, v+1, v+2);
        for (int i = 0; i < 3; ++i)v[i] = (v[i] - r[i]) * 1e6;
        if (r[0] < 1.05)break;
        if (a*a + r[0] * r[0] - 2 * r[0] < 0.1)break;
        double ds2 = 0.0;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                ds2 += g_sph_table[i][j](r[0], r[1], r[2], r[3]) * v[i] * v[j];

        // 计算 ds2 对 v[3] 的导数
        double d_ds2_d_v3 = 0.0;
        for (int j = 0; j < 4; ++j)
            d_ds2_d_v3 += g_sph_table[3][j](r[0], r[1], r[2], r[3]) * v[j];
        d_ds2_d_v3 *= 2.0;

        // 避免除零
        if (std::abs(d_ds2_d_v3) > 1e-12) {
            double alpha = 1.0; // 可调节
            v[3] -= alpha * ds2 / d_ds2_d_v3;
        }
        Eigen::Vector3d e_r(
            std::sin(r[1]) * std::cos(r[2]),
            std::sin(r[1]) * std::sin(r[2]),
            std::cos(r[1])
        );
        Eigen::Vector3d e_theta(
            std::cos(r[1]) * std::cos(r[2]),
            std::cos(r[1]) * std::sin(r[2]),
            -std::sin(r[1])
        );
        Eigen::Vector3d e_phi(
            -std::sin(r[2]),
            std::cos(r[2]),
            0.0
        );
        double step1 = (pow(x0[2] * M_,2)<0.12*0.12)?0.2*step:step;
        step1 *= min(pow(x0.norm()/5.0,2),1.0);
        double acc[4] = {0,0,0,0};
        for (int i = 0; i < 4; ++i){
            for (int j = 0; j < 4; ++j){
                for (int k = 0; k < 4; ++k){
                    //double theta = (r[1] == pi)?(1.5707):r[1];
                    acc[i] -= gamma_sph_table[i][j][k](r[0], r[1], r[2], r[3]) * v[j] * v[k];
                }
            }
            //std::cout << i << ' ' << r[i] << ' ';
        }
        v0 += step1 * (acc[0] * e_r + acc[1] * r[0] * e_theta + acc[2] * r[0] * std::sin(r[1]) * e_phi);
        x0 += step1 * v0;
        v[3] += acc[3] * step1;
        r[3] += v[3] * step1;
        //std::cout << x0 << ' ' << r[3] << std::endl;
        if (pow(x0[2] * M_ ,2) <0.2 ){
            new_light->push_back(x0 * M_);
            new_l->push_back(v0.norm() * step1 * M_);
        }
    }
    l_ = new_l;
    light = new_light;
    //std::cout << light->size() << std::endl;
    return light;
}
sv Gauge_March::compute_l(){
    return l_;
}