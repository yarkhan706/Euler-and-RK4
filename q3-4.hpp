#ifndef Q3_4_HPP
#define Q3_4_HPP

#include <valarray>
#include <array>

// Speed of light in meters per second
const double c = 299792458.0;

// Function prototypes
void f(std::array<double, 2> &result, const std::array<double, 2> &x);
void f_Jac(std::array<std::array<double, 2>, 2> &jacobian, const std::array<double, 2> &x);

// Function prototypes
std::valarray<double> rhs(double t, std::valarray<double> yvec);
std::valarray<double> euler(double t0, std::valarray<double> y0, double h, double tf);
std::valarray<double> rungeKutta4(double t0, std::valarray<double> y0, double h, double tf);

// Satellite positions and measured times
extern std::array<double, 2> xA, xB;
extern double tA, tB;

#endif // Q3_4_HPP
