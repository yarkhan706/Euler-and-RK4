#include <iostream>
#include <cmath>
#include <valarray>
#include "q3-4.hpp"

using namespace std;

// Define the RHS function for ODEs
valarray<double> rhs(double t, valarray<double> yvec) {
    // Implement the RHS of the ODEs here
    // Example:
    valarray<double> dydt(yvec.size());
    dydt[0] = t * yvec[0]; // Example ODE
    return dydt;
}

// Runge-Kutta 4th order method
valarray<double> rungeKutta4(double t0, valarray<double> y0, double h, double tf) {
    double t = t0;
    valarray<double> y = y0;
    valarray<double> k1, k2, k3, k4;

    // Evolution loop for 4th order Runge-Kutta
    while (t < tf) {
        k1 = h * rhs(t, y);
        k2 = h * rhs(t + h / 2.0, y + k1 / 2.0);
        k3 = h * rhs(t + h / 2.0, y + k2 / 2.0);
        k4 = h * rhs(t + h, y + k3);

        // Update y using the weighted average of slopes
        y += (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;

        // Move to the next step
        t += h;
    }

    return y;
}
