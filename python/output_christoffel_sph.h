#include <cmath>
#define a 1.5
using namespace std;
template <int Index> double gamma_sph(double r, double theta, double phi, double t);
template <> inline double gamma_sph<0>(double r, double theta, double phi, double t) { return (r*(pow(a, 2) + pow(r, 2) - 2*r) + (1 - r)*(pow(a, 2)*pow(cos(theta), 2) + pow(r, 2)))*(1.0*pow(a, 2) + 1.0*pow(r, 2) - 2.0*r)/((pow(a, 2)*pow(cos(theta), 2) + pow(r, 2))*pow(pow(a, 2) + pow(r, 2) - 2*r, 2)); }
template <> inline double gamma_sph<1>(double r, double theta, double phi, double t) { return -1.0*pow(a, 2)*sin(2*theta)/(pow(a, 2)*cos(2*theta) + pow(a, 2) + 2*pow(r, 2)); }
template <> inline double gamma_sph<2>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<3>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<4>(double r, double theta, double phi, double t) { return -1.0*pow(a, 2)*sin(2*theta)/(pow(a, 2)*cos(2*theta) + pow(a, 2) + 2*pow(r, 2)); }
template <> inline double gamma_sph<5>(double r, double theta, double phi, double t) { return 1.0*r*(-pow(a, 2)*pow(sin(theta), 2) + 2*r)/(pow(a, 2)*pow(cos(theta), 2) + pow(r, 2)); }
template <> inline double gamma_sph<6>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<7>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<8>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<9>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<10>(double r, double theta, double phi, double t) { return 1.0*(r*pow(pow(a, 2)*pow(cos(theta), 2) + pow(r, 2), 3) + (-pow(a, 2) - pow(r, 2) + 2*r)*(-2*pow(a, 2)*pow(r, 2)*pow(sin(theta), 2) + pow(a, 2)*(pow(a, 2)*pow(cos(theta), 2) + pow(r, 2))*pow(sin(theta), 2) + r*pow(pow(a, 2)*pow(cos(theta), 2) + pow(r, 2), 2)))*pow(sin(theta), 2)/pow(pow(a, 2)*pow(cos(theta), 2) + pow(r, 2), 3); }
template <> inline double gamma_sph<11>(double r, double theta, double phi, double t) { return 1.0*a*(pow(a, 2)*pow(cos(theta), 2) - pow(r, 2))*(pow(a, 2) + pow(r, 2) - 2*r)*pow(sin(theta), 2)/pow(pow(a, 2)*pow(cos(theta), 2) + pow(r, 2), 3); }
template <> inline double gamma_sph<12>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<13>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<14>(double r, double theta, double phi, double t) { return 1.0*a*(pow(a, 2)*pow(cos(theta), 2) - pow(r, 2))*(pow(a, 2) + pow(r, 2) - 2*r)*pow(sin(theta), 2)/pow(pow(a, 2)*pow(cos(theta), 2) + pow(r, 2), 3); }
template <> inline double gamma_sph<15>(double r, double theta, double phi, double t) { return (-pow(a, 2)*pow(cos(theta), 2) + pow(r, 2))*(1.0*pow(a, 2) + 1.0*pow(r, 2) - 2.0*r)/pow(pow(a, 2)*pow(cos(theta), 2) + pow(r, 2), 3); }
template <> inline double gamma_sph<16>(double r, double theta, double phi, double t) { return 1.0*pow(a, 2)*sin(2*theta)/((pow(a, 2) + pow(r, 2) - 2*r)*(pow(a, 2)*cos(2*theta) + pow(a, 2) + 2*pow(r, 2))); }
template <> inline double gamma_sph<17>(double r, double theta, double phi, double t) { return -1.0*pow(a, 2)*pow(cos(theta), 2)/(r*(pow(a, 2)*pow(cos(theta), 2) + pow(r, 2))); }
template <> inline double gamma_sph<18>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<19>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<20>(double r, double theta, double phi, double t) { return -1.0*pow(a, 2)*pow(cos(theta), 2)/(r*(pow(a, 2)*pow(cos(theta), 2) + pow(r, 2))); }
template <> inline double gamma_sph<21>(double r, double theta, double phi, double t) { return -1.0*pow(a, 2)*sin(2*theta)/(pow(a, 2)*cos(2*theta) + pow(a, 2) + 2*pow(r, 2)); }
template <> inline double gamma_sph<22>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<23>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<24>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<25>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<26>(double r, double theta, double phi, double t) { return (-2.0*pow(a, 2)*r*(pow(a, 2) + pow(r, 2))*pow(sin(theta), 2) + 1.0*pow(pow(a, 2)*pow(cos(theta), 2) + pow(r, 2), 3) - 1.0*(pow(a, 2)*pow(cos(theta), 2) + pow(r, 2))*(2*pow(a, 2)*r*pow(sin(theta), 2) + (pow(a, 2) + pow(r, 2))*(pow(a, 2)*pow(cos(theta), 2) + pow(r, 2))))*sin(theta)*cos(theta)/pow(pow(a, 2)*pow(cos(theta), 2) + pow(r, 2), 3); }
template <> inline double gamma_sph<27>(double r, double theta, double phi, double t) { return 8.0*a*r*(pow(a, 2) + pow(r, 2))*sin(2*theta)/pow(pow(a, 2)*cos(2*theta) + pow(a, 2) + 2*pow(r, 2), 3); }
template <> inline double gamma_sph<28>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<29>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<30>(double r, double theta, double phi, double t) { return 8.0*a*r*(pow(a, 2) + pow(r, 2))*sin(2*theta)/pow(pow(a, 2)*cos(2*theta) + pow(a, 2) + 2*pow(r, 2), 3); }
template <> inline double gamma_sph<31>(double r, double theta, double phi, double t) { return -8.0*pow(a, 2)*r*sin(2*theta)/pow(pow(a, 2)*cos(2*theta) + pow(a, 2) + 2*pow(r, 2), 3); }
template <> inline double gamma_sph<32>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<33>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<34>(double r, double theta, double phi, double t) { return pow(a, 2)*(-1.0*pow(a, 4)*pow(cos(theta), 4) - 2.0*pow(a, 2)*pow(r, 2)*pow(cos(theta), 2) - 0.125*pow(a, 2)*r*(1 - cos(4*theta)) + 2.0*pow(a, 2)*r*pow(cos(theta), 2) - 1.0*pow(r, 4) - 3.0*pow(r, 3)*pow(sin(theta), 2) + 2.0*pow(r, 3))/(r*pow(pow(a, 2)*pow(cos(theta), 2) + pow(r, 2), 2)*(1.0*pow(a, 2) + 1.0*pow(r, 2) - 2.0*r)); }
template <> inline double gamma_sph<35>(double r, double theta, double phi, double t) { return 2.0*a*(-pow(a, 2)*cos(2*theta) - pow(a, 2) + 2*pow(r, 2))/((pow(a, 2) + pow(r, 2) - 2*r)*pow(pow(a, 2)*cos(2*theta) + pow(a, 2) + 2*pow(r, 2), 2)); }
template <> inline double gamma_sph<36>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<37>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<38>(double r, double theta, double phi, double t) { return 2*pow(a, 2)*r*(2.0*pow(a, 2) + 2.0*pow(r, 2) - 4.0*r)*sin(2*theta)/((1.0*pow(a, 2) + 1.0*pow(r, 2) - 2.0*r)*pow(pow(a, 2)*cos(2*theta) + pow(a, 2) + 2*pow(r, 2), 2)); }
template <> inline double gamma_sph<39>(double r, double theta, double phi, double t) { return -2.0*a*r/((1.0*pow(a, 4)*pow(cos(theta), 4) + 2.0*pow(a, 2)*pow(r, 2)*pow(cos(theta), 2) + 1.0*pow(r, 4))*tan(theta)); }
template <> inline double gamma_sph<40>(double r, double theta, double phi, double t) { return pow(a, 2)*(-1.0*pow(a, 4)*pow(cos(theta), 4) - 2.0*pow(a, 2)*pow(r, 2)*pow(cos(theta), 2) - 0.125*pow(a, 2)*r*(1 - cos(4*theta)) + 2.0*pow(a, 2)*r*pow(cos(theta), 2) - 1.0*pow(r, 4) - 3.0*pow(r, 3)*pow(sin(theta), 2) + 2.0*pow(r, 3))/(r*pow(pow(a, 2)*pow(cos(theta), 2) + pow(r, 2), 2)*(1.0*pow(a, 2) + 1.0*pow(r, 2) - 2.0*r)); }
template <> inline double gamma_sph<41>(double r, double theta, double phi, double t) { return 2*pow(a, 2)*r*(2.0*pow(a, 2) + 2.0*pow(r, 2) - 4.0*r)*sin(2*theta)/((1.0*pow(a, 2) + 1.0*pow(r, 2) - 2.0*r)*pow(pow(a, 2)*cos(2*theta) + pow(a, 2) + 2*pow(r, 2), 2)); }
template <> inline double gamma_sph<42>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<43>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<44>(double r, double theta, double phi, double t) { return 2.0*a*(-pow(a, 2)*cos(2*theta) - pow(a, 2) + 2*pow(r, 2))/((pow(a, 2) + pow(r, 2) - 2*r)*pow(pow(a, 2)*cos(2*theta) + pow(a, 2) + 2*pow(r, 2), 2)); }
template <> inline double gamma_sph<45>(double r, double theta, double phi, double t) { return -2.0*a*r/((1.0*pow(a, 4)*pow(cos(theta), 4) + 2.0*pow(a, 2)*pow(r, 2)*pow(cos(theta), 2) + 1.0*pow(r, 4))*tan(theta)); }
template <> inline double gamma_sph<46>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<47>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<48>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<49>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<50>(double r, double theta, double phi, double t) { return a*(1.0*pow(a, 4)*pow(cos(theta), 2) - 1.0*pow(a, 2)*pow(r, 2)*pow(cos(theta), 2) - 1.0*pow(a, 2)*pow(r, 2) - 3.0*pow(r, 4))*pow(sin(theta), 2)/(pow(pow(a, 2)*pow(cos(theta), 2) + pow(r, 2), 2)*(1.0*pow(a, 2) + 1.0*pow(r, 2) - 2.0*r)); }
template <> inline double gamma_sph<51>(double r, double theta, double phi, double t) { return 1.0*(-pow(a, 4)*pow(cos(theta), 2) - pow(a, 2)*pow(r, 2)*pow(cos(theta), 2) + pow(a, 2)*pow(r, 2) + pow(r, 4))/(pow(pow(a, 2)*pow(cos(theta), 2) + pow(r, 2), 2)*(pow(a, 2) + pow(r, 2) - 2*r)); }
template <> inline double gamma_sph<52>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<53>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<54>(double r, double theta, double phi, double t) { return 2.0*pow(a, 3)*r*pow(sin(theta), 3)*cos(theta)/(1.0*pow(a, 4)*pow(cos(theta), 4) + 2.0*pow(a, 2)*pow(r, 2)*pow(cos(theta), 2) + 1.0*pow(r, 4)); }
template <> inline double gamma_sph<55>(double r, double theta, double phi, double t) { return pow(a, 2)*r*(-4.0*pow(a, 2) - 4.0*pow(r, 2) + 8.0*r)*sin(2*theta)/((pow(a, 2) + pow(r, 2) - 2*r)*pow(pow(a, 2)*cos(2*theta) + pow(a, 2) + 2*pow(r, 2), 2)); }
template <> inline double gamma_sph<56>(double r, double theta, double phi, double t) { return a*(1.0*pow(a, 4)*pow(cos(theta), 2) - 1.0*pow(a, 2)*pow(r, 2)*pow(cos(theta), 2) - 1.0*pow(a, 2)*pow(r, 2) - 3.0*pow(r, 4))*pow(sin(theta), 2)/(pow(pow(a, 2)*pow(cos(theta), 2) + pow(r, 2), 2)*(1.0*pow(a, 2) + 1.0*pow(r, 2) - 2.0*r)); }
template <> inline double gamma_sph<57>(double r, double theta, double phi, double t) { return 2.0*pow(a, 3)*r*pow(sin(theta), 3)*cos(theta)/(1.0*pow(a, 4)*pow(cos(theta), 4) + 2.0*pow(a, 2)*pow(r, 2)*pow(cos(theta), 2) + 1.0*pow(r, 4)); }
template <> inline double gamma_sph<58>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<59>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<60>(double r, double theta, double phi, double t) { return 1.0*(-pow(a, 4)*pow(cos(theta), 2) - pow(a, 2)*pow(r, 2)*pow(cos(theta), 2) + pow(a, 2)*pow(r, 2) + pow(r, 4))/(pow(pow(a, 2)*pow(cos(theta), 2) + pow(r, 2), 2)*(pow(a, 2) + pow(r, 2) - 2*r)); }
template <> inline double gamma_sph<61>(double r, double theta, double phi, double t) { return pow(a, 2)*r*(-4.0*pow(a, 2) - 4.0*pow(r, 2) + 8.0*r)*sin(2*theta)/((pow(a, 2) + pow(r, 2) - 2*r)*pow(pow(a, 2)*cos(2*theta) + pow(a, 2) + 2*pow(r, 2), 2)); }
template <> inline double gamma_sph<62>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<63>(double r, double theta, double phi, double t) { return 0; }
