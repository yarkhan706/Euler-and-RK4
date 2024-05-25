#ifndef INTERP_HPP
#define INTERP_HPP

#include <valarray>

double lagrange_interp(const std::valarray<double>& x, const std::valarray<double>& y, double x_val);

#endif // INTERP_HPP
