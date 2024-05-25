#include <iostream>
#include <fstream>
#include <cmath>
#include <valarray>
#include "q3-4.hpp"

using namespace std;

const double G = 6.67430e-11; // Gravitational constant
const double M = 5.972e24;     // Mass of the Earth

// Define the RHS function for ODEs
valarray<double> rhs(double t, valarray<double> yvec) {
    // Implement the RHS of the ODEs here
    valarray<double> dydt(yvec.size());

    double GM = G * M;
    double x = yvec[0];
    double y = yvec[1];
    double vx = yvec[2];
    double vy = yvec[3];

    double r = sqrt(x * x + y * y);
    double r_cubed = r * r * r;

    dydt[0] = vx;
    dydt[1] = vy;
    dydt[2] = -GM * x / r_cubed;
    dydt[3] = -GM * y / r_cubed;

    return dydt;
}

// Euler's method
valarray<double> euler(double t0, valarray<double> y0, double h, double tf) {
    double t = t0;
    valarray<double> y = y0;

    // Evolution loop for Euler's method
    ofstream outFile("euler_output86.dat");
    while (t < tf) {
        y += h * rhs(t, y);
        t += h;
        if (fmod(t, 60.0) == 0) { // Print every 60 seconds
            outFile << t << " " << y[0] << " " << y[1] << " " << y[2] << " " << y[3] << endl;
        }
    }
    outFile.close();

    return y;
}

// Runge-Kutta 4th order method
valarray<double> rungeKutta4(double t0, valarray<double> y0, double h, double tf) {
    double t = t0;
    valarray<double> y = y0;
    valarray<double> k1, k2, k3, k4;

    // Evolution loop for 4th order Runge-Kutta
    ofstream outFile("rk4_output86.dat");
    while (t < tf) {
        k1 = h * rhs(t, y);
        k2 = h * rhs(t + h / 2.0, y + k1 / 2.0);
        k3 = h * rhs(t + h / 2.0, y + k2 / 2.0);
        k4 = h * rhs(t + h, y + k3);

        // Update y using the weighted average of slopes
        y += (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;

        // Move to the next step
        t += h;

        if (fmod(t, 60.0) == 0) { // Print every 60 seconds
            outFile << t << " " << y[0] << " " << y[1] << " " << y[2] << " " << y[3] << endl;
        }
    }
    outFile.close();

    return y;
}

int main() {
    // Initial conditions
    double t0 = 0.0;
    valarray<double> y0 = {0.0, 26378100.0, 3887.3, 0.0}; // x0, y0, vx0, vy0
    double h = 60.0; // Time step (60 seconds)
    double tf = 86400.0; // Final time (86400 seconds)

    // Solve using Euler's method
    valarray<double> y_euler = euler(t0, y0, h, tf);

    // Solve using Runge-Kutta 4th order method
    valarray<double> y_rk4 = rungeKutta4(t0, y0, h, tf);

    return 0;
}
