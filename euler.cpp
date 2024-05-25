#include <iostream>
#include <cmath>
#include <valarray>
#include "q3-4.hpp"

using namespace std;


// Euler's method
valarray<double> euler(double t0, valarray<double> y0, double h, double tf) {
    double t = t0;
    valarray<double> y = y0;

    // Evolution loop for Euler's method
    while (t < tf) {
        y += h * rhs(t, y);
        t += h;
    }

    return y;
}

