#include "interp.hpp"

/* Function to perform Lagrange interpolation for given data points */
double lagrange_interp(const std::valarray<double>& x, const std::valarray<double>& y, double x_val) {
    double result = 0.0;
    size_t n = x.size();
    for (size_t i = 0; i < n; ++i) {
        double term = y[i];
        for (size_t j = 0; j < n; ++j) {
            if (j != i) {
                term *= (x_val - x[j]) / (x[i] - x[j]);
            }
        }
        result += term;
    }
    return result;
}
