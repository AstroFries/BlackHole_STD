#include <cmath>
using namespace std;
template <int Index> double gamma_sph(double r, double theta, double phi, double t);
template <> inline double gamma_sph<0>(double r, double theta, double phi, double t) { return (r*(9*pow(r, 2) - 18*r + 1) + (1 - r)*(9*pow(r, 2) + pow(cos(theta), 2)))*(81.0*pow(r, 2) - 162.0*r + 9.0)/((9*pow(r, 2) + pow(cos(theta), 2))*pow(9*pow(r, 2) - 18*r + 1, 2)); }
template <> inline double gamma_sph<1>(double r, double theta, double phi, double t) { return -1.0*sin(2*theta)/(18*pow(r, 2) + cos(2*theta) + 1); }
template <> inline double gamma_sph<2>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<3>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<4>(double r, double theta, double phi, double t) { return -1.0*sin(2*theta)/(18*pow(r, 2) + cos(2*theta) + 1); }
template <> inline double gamma_sph<5>(double r, double theta, double phi, double t) { return r*(-9.0*pow(r, 2) + 18.0*r - 1.0)/(9.0*pow(r, 2) + 1.0*pow(cos(theta), 2)); }
template <> inline double gamma_sph<6>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<7>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<8>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<9>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<10>(double r, double theta, double phi, double t) { return (-9.0*pow(r, 2) + 18.0*r - 1.0)*(18*pow(r, 2)*(-9*pow(r, 2) + pow(sin(theta), 2) - 1)*pow(sin(theta), 2) + r*pow(9*pow(r, 2) + pow(cos(theta), 2), 3) + pow(9*pow(r, 2) + pow(cos(theta), 2), 2)*pow(sin(theta), 2))*pow(sin(theta), 2)/pow(9*pow(r, 2) + pow(cos(theta), 2), 4); }
template <> inline double gamma_sph<11>(double r, double theta, double phi, double t) { return (9*pow(r, 2) - pow(cos(theta), 2))*(-27.0*pow(r, 2) + 54.0*r - 3.0)*pow(sin(theta), 2)/pow(9*pow(r, 2) + pow(cos(theta), 2), 3); }
template <> inline double gamma_sph<12>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<13>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<14>(double r, double theta, double phi, double t) { return (9*pow(r, 2) - pow(cos(theta), 2))*(-27.0*pow(r, 2) + 54.0*r - 3.0)*pow(sin(theta), 2)/pow(9*pow(r, 2) + pow(cos(theta), 2), 3); }
template <> inline double gamma_sph<15>(double r, double theta, double phi, double t) { return (9*pow(r, 2) - pow(cos(theta), 2))*(81.0*pow(r, 2) - 162.0*r + 9.0)/pow(9*pow(r, 2) + pow(cos(theta), 2), 3); }
template <> inline double gamma_sph<16>(double r, double theta, double phi, double t) { return 9.0*sin(2*theta)/((9*pow(r, 2) - 18*r + 1)*(18*pow(r, 2) + cos(2*theta) + 1)); }
template <> inline double gamma_sph<17>(double r, double theta, double phi, double t) { return 9.0*r/(9*pow(r, 2) + pow(cos(theta), 2)); }
template <> inline double gamma_sph<18>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<19>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<20>(double r, double theta, double phi, double t) { return 9.0*r/(9*pow(r, 2) + pow(cos(theta), 2)); }
template <> inline double gamma_sph<21>(double r, double theta, double phi, double t) { return -1.0*sin(2*theta)/(18*pow(r, 2) + cos(2*theta) + 1); }
template <> inline double gamma_sph<22>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<23>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<24>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<25>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<26>(double r, double theta, double phi, double t) { return (1.0/9.0)*(-162.0*r*(9*pow(r, 2) + 1)*pow(sin(theta), 2) - 9.0*(9*pow(r, 2) + pow(cos(theta), 2))*(18*r*pow(sin(theta), 2) + (9*pow(r, 2) + 1)*(9*pow(r, 2) + pow(cos(theta), 2))))*sin(theta)*cos(theta)/pow(9*pow(r, 2) + pow(cos(theta), 2), 3); }
template <> inline double gamma_sph<27>(double r, double theta, double phi, double t) { return 216.0*r*(9*pow(r, 2) + 1)*sin(2*theta)/pow(18*pow(r, 2) + cos(2*theta) + 1, 3); }
template <> inline double gamma_sph<28>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<29>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<30>(double r, double theta, double phi, double t) { return 216.0*r*(9*pow(r, 2) + 1)*sin(2*theta)/pow(18*pow(r, 2) + cos(2*theta) + 1, 3); }
template <> inline double gamma_sph<31>(double r, double theta, double phi, double t) { return -648.0*r*sin(2*theta)/pow(18*pow(r, 2) + cos(2*theta) + 1, 3); }
template <> inline double gamma_sph<32>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<33>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<34>(double r, double theta, double phi, double t) { return (-162.0*r*(81*pow(r, 4)*pow(sin(theta), 2) - pow(1 - pow(sin(theta), 2), 2) + pow(cos(theta), 6)) + 9.0*(9*pow(r, 2) - 18*r + pow(cos(theta), 2))*(18*pow(r, 2)*(-9*pow(r, 2) + pow(sin(theta), 2) - 1)*pow(sin(theta), 2) + r*pow(9*pow(r, 2) + pow(cos(theta), 2), 3) + pow(9*pow(r, 2) + pow(cos(theta), 2), 2)*pow(sin(theta), 2)))/(pow(9*pow(r, 2) + pow(cos(theta), 2), 3)*(81*pow(r, 4) - 162*pow(r, 3) + 9*pow(r, 2)*pow(cos(theta), 2) + 9*pow(r, 2) - 18*r*pow(cos(theta), 2) + pow(cos(theta), 2))); }
template <> inline double gamma_sph<35>(double r, double theta, double phi, double t) { return (2187.0*pow(r, 4) - 27.0*pow(cos(theta), 4))/(pow(9*pow(r, 2) + pow(cos(theta), 2), 3)*(9*pow(r, 2) - 18*r + 1)); }
template <> inline double gamma_sph<36>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<37>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<38>(double r, double theta, double phi, double t) { return (1.0/9.0)*(pow(r, 2)*(26244.0*pow(r, 2) + 2916.0)*pow(sin(theta), 2) + 9.0*(18*r*(9*pow(r, 2) + 1)*pow(sin(theta), 2) + (9*pow(r, 2) + pow(cos(theta), 2))*(18*r*pow(sin(theta), 2) + (9*pow(r, 2) + 1)*(9*pow(r, 2) + pow(cos(theta), 2))))*(9*pow(r, 2) - 18*r + pow(cos(theta), 2)))/(pow(9*pow(r, 2) + pow(cos(theta), 2), 2)*(81*pow(r, 4) - 162*pow(r, 3) + 9*pow(r, 2)*pow(cos(theta), 2) + 9*pow(r, 2) - 18*r*pow(cos(theta), 2) + pow(cos(theta), 2))*tan(theta)); }
template <> inline double gamma_sph<39>(double r, double theta, double phi, double t) { return -4374.0*r/((6561.0*pow(r, 4) + 1458.0*pow(r, 2)*pow(cos(theta), 2) + 81.0*pow(cos(theta), 4))*tan(theta)); }
template <> inline double gamma_sph<40>(double r, double theta, double phi, double t) { return (-162.0*r*(81*pow(r, 4)*pow(sin(theta), 2) - pow(1 - pow(sin(theta), 2), 2) + pow(cos(theta), 6)) + 9.0*(9*pow(r, 2) - 18*r + pow(cos(theta), 2))*(18*pow(r, 2)*(-9*pow(r, 2) + pow(sin(theta), 2) - 1)*pow(sin(theta), 2) + r*pow(9*pow(r, 2) + pow(cos(theta), 2), 3) + pow(9*pow(r, 2) + pow(cos(theta), 2), 2)*pow(sin(theta), 2)))/(pow(9*pow(r, 2) + pow(cos(theta), 2), 3)*(81*pow(r, 4) - 162*pow(r, 3) + 9*pow(r, 2)*pow(cos(theta), 2) + 9*pow(r, 2) - 18*r*pow(cos(theta), 2) + pow(cos(theta), 2))); }
template <> inline double gamma_sph<41>(double r, double theta, double phi, double t) { return (1.0/9.0)*(pow(r, 2)*(26244.0*pow(r, 2) + 2916.0)*pow(sin(theta), 2) + 9.0*(18*r*(9*pow(r, 2) + 1)*pow(sin(theta), 2) + (9*pow(r, 2) + pow(cos(theta), 2))*(18*r*pow(sin(theta), 2) + (9*pow(r, 2) + 1)*(9*pow(r, 2) + pow(cos(theta), 2))))*(9*pow(r, 2) - 18*r + pow(cos(theta), 2)))/(pow(9*pow(r, 2) + pow(cos(theta), 2), 2)*(81*pow(r, 4) - 162*pow(r, 3) + 9*pow(r, 2)*pow(cos(theta), 2) + 9*pow(r, 2) - 18*r*pow(cos(theta), 2) + pow(cos(theta), 2))*tan(theta)); }
template <> inline double gamma_sph<42>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<43>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<44>(double r, double theta, double phi, double t) { return (2187.0*pow(r, 4) - 27.0*pow(cos(theta), 4))/(pow(9*pow(r, 2) + pow(cos(theta), 2), 3)*(9*pow(r, 2) - 18*r + 1)); }
template <> inline double gamma_sph<45>(double r, double theta, double phi, double t) { return -4374.0*r/((6561.0*pow(r, 4) + 1458.0*pow(r, 2)*pow(cos(theta), 2) + 81.0*pow(cos(theta), 4))*tan(theta)); }
template <> inline double gamma_sph<46>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<47>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<48>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<49>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<50>(double r, double theta, double phi, double t) { return (-6561.0*pow(r, 4) - 243.0*pow(r, 2)*pow(cos(theta), 2) - 243.0*pow(r, 2) + 27.0*pow(cos(theta), 2))*pow(sin(theta), 2)/(pow(1.0*pow(r, 2) + 0.1111111111111111*pow(cos(theta), 2), 2)*(6561.0*pow(r, 2) - 13122.0*r + 729.0)); }
template <> inline double gamma_sph<51>(double r, double theta, double phi, double t) { return (6561.0*pow(r, 6) + 729.0*pow(r, 4) - 81.0*pow(r, 2)*pow(cos(theta), 4) - 9.0*pow(cos(theta), 4))/(pow(9*pow(r, 2) + pow(cos(theta), 2), 3)*(9*pow(r, 2) - 18*r + 1)); }
template <> inline double gamma_sph<52>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<53>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<54>(double r, double theta, double phi, double t) { return 486.0*r*pow(sin(theta), 3)*cos(theta)/(6561.0*pow(r, 4) + 1458.0*pow(r, 2)*pow(cos(theta), 2) + 81.0*pow(cos(theta), 4)); }
template <> inline double gamma_sph<55>(double r, double theta, double phi, double t) { return -1458.0*r*sin(theta)*cos(theta)/(6561.0*pow(r, 4) + 1458.0*pow(r, 2)*pow(cos(theta), 2) + 81.0*pow(cos(theta), 4)); }
template <> inline double gamma_sph<56>(double r, double theta, double phi, double t) { return (-6561.0*pow(r, 4) - 243.0*pow(r, 2)*pow(cos(theta), 2) - 243.0*pow(r, 2) + 27.0*pow(cos(theta), 2))*pow(sin(theta), 2)/(pow(1.0*pow(r, 2) + 0.1111111111111111*pow(cos(theta), 2), 2)*(6561.0*pow(r, 2) - 13122.0*r + 729.0)); }
template <> inline double gamma_sph<57>(double r, double theta, double phi, double t) { return 486.0*r*pow(sin(theta), 3)*cos(theta)/(6561.0*pow(r, 4) + 1458.0*pow(r, 2)*pow(cos(theta), 2) + 81.0*pow(cos(theta), 4)); }
template <> inline double gamma_sph<58>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<59>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<60>(double r, double theta, double phi, double t) { return (6561.0*pow(r, 6) + 729.0*pow(r, 4) - 81.0*pow(r, 2)*pow(cos(theta), 4) - 9.0*pow(cos(theta), 4))/(pow(9*pow(r, 2) + pow(cos(theta), 2), 3)*(9*pow(r, 2) - 18*r + 1)); }
template <> inline double gamma_sph<61>(double r, double theta, double phi, double t) { return -1458.0*r*sin(theta)*cos(theta)/(6561.0*pow(r, 4) + 1458.0*pow(r, 2)*pow(cos(theta), 2) + 81.0*pow(cos(theta), 4)); }
template <> inline double gamma_sph<62>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<63>(double r, double theta, double phi, double t) { return 0; }
