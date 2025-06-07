#include <cmath>
#define a 1.5
using namespace std;
template <int Index> double g_sph(double r, double theta, double phi, double t);
template <> inline double g_sph<0>(double r, double theta, double phi, double t) { return (pow(a, 2)*pow(cos(theta), 2) + pow(r, 2))/(pow(a, 2) + pow(r, 2) - 2*r); }
template <> inline double g_sph<1>(double r, double theta, double phi, double t) { return 0; }
template <> inline double g_sph<2>(double r, double theta, double phi, double t) { return 0; }
template <> inline double g_sph<3>(double r, double theta, double phi, double t) { return 0; }
template <> inline double g_sph<4>(double r, double theta, double phi, double t) { return 0; }
template <> inline double g_sph<5>(double r, double theta, double phi, double t) { return pow(a, 2)*pow(cos(theta), 2) + pow(r, 2); }
template <> inline double g_sph<6>(double r, double theta, double phi, double t) { return 0; }
template <> inline double g_sph<7>(double r, double theta, double phi, double t) { return 0; }
template <> inline double g_sph<8>(double r, double theta, double phi, double t) { return 0; }
template <> inline double g_sph<9>(double r, double theta, double phi, double t) { return 0; }
template <> inline double g_sph<10>(double r, double theta, double phi, double t) { return (2*pow(a, 2)*r*pow(sin(theta), 2)/(pow(a, 2)*pow(cos(theta), 2) + pow(r, 2)) + pow(a, 2) + pow(r, 2))*pow(sin(theta), 2); }
template <> inline double g_sph<11>(double r, double theta, double phi, double t) { return -2*a*r*pow(sin(theta), 2)/(pow(a, 2)*pow(cos(theta), 2) + pow(r, 2)); }
template <> inline double g_sph<12>(double r, double theta, double phi, double t) { return 0; }
template <> inline double g_sph<13>(double r, double theta, double phi, double t) { return 0; }
template <> inline double g_sph<14>(double r, double theta, double phi, double t) { return -2*a*r*pow(sin(theta), 2)/(pow(a, 2)*pow(cos(theta), 2) + pow(r, 2)); }
template <> inline double g_sph<15>(double r, double theta, double phi, double t) { return 2*r/(pow(a, 2)*pow(cos(theta), 2) + pow(r, 2)) - 1; }
