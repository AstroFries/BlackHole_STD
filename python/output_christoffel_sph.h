#include <cmath>
using namespace std;
template <int Index> double gamma_sph(double r, double theta, double phi, double t);
template <> inline double gamma_sph<0>(double r, double theta, double phi, double t) { return -1.0/(r*(r - 2)); }
template <> inline double gamma_sph<1>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<2>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<3>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<4>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<5>(double r, double theta, double phi, double t) { return 2.0; }
template <> inline double gamma_sph<6>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<7>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<8>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<9>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<10>(double r, double theta, double phi, double t) { return 2.0*pow(sin(theta), 2); }
template <> inline double gamma_sph<11>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<12>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<13>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<14>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<15>(double r, double theta, double phi, double t) { return (1.0*r - 2.0)/pow(r, 3); }
template <> inline double gamma_sph<16>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<17>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<18>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<19>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<20>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<21>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<22>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<23>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<24>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<25>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<26>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<27>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<28>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<29>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<30>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<31>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<32>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<33>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<34>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<35>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<36>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<37>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<38>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<39>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<40>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<41>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<42>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<43>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<44>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<45>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<46>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<47>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<48>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<49>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<50>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<51>(double r, double theta, double phi, double t) { return 1.0/(r*(r - 2)); }
template <> inline double gamma_sph<52>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<53>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<54>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<55>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<56>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<57>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<58>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<59>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<60>(double r, double theta, double phi, double t) { return 1.0/(r*(r - 2)); }
template <> inline double gamma_sph<61>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<62>(double r, double theta, double phi, double t) { return 0; }
template <> inline double gamma_sph<63>(double r, double theta, double phi, double t) { return 0; }
