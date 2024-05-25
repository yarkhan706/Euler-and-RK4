#include <iostream>
#include <fstream>
#include <cmath>
#include <valarray>

using namespace std;

const double G = 6.67430e-11; // Gravitational constant
const double M = 5.972e24;     // Mass of the Earth
const double ML = 7.342e22;    // Mass of the Moon

// Define the RHS function for ODEs with Moon's gravitational effect
valarray<double> rhs_with_moon(double t, valarray<double> yvec) {
    // Implement the RHS of the ODEs here
    valarray<double> dydt(yvec.size());

    double GM = G * M; // Earth's gravitational parameter
    double GM_L = G * ML; // Moon's gravitational parameter
    double x = yvec[0];
    double y = yvec[1];
    double vx = yvec[2];
    double vy = yvec[3];

    double r = sqrt(x * x + y * y);
    double r_cubed = r * r * r;

    // Position of the Moon
    double xL = 384400000.0; // x position of the Moon
    double yL = 0.0; // y position of the Moon

    // Distance between the satellite and the Moon
    double dx = x - xL;
    double dy = y - yL;
    double r_moon = sqrt(dx * dx + dy * dy);
    double r_moon_cubed = r_moon * r_moon * r_moon;

    // Gravitational effect of the Moon
    double ax_moon = -GM_L * dx / r_moon_cubed;
    double ay_moon = -GM_L * dy / r_moon_cubed;

    dydt[0] = vx;
    dydt[1] = vy;
    dydt[2] = (-GM * x / r_cubed) + ax_moon;
    dydt[3] = (-GM * y / r_cubed) + ay_moon;

    return dydt;
}

// Runge-Kutta 4th order method
valarray<double> rungeKutta4(double t0, valarray<double> y0, double h, double tf) {
    double t = t0;
    valarray<double> y = y0;
    valarray<double> k1, k2, k3, k4;

    // Evolution loop for 4th order Runge-Kutta
    ofstream outFile("rk4_outputMoon.dat");
    while (t < tf) {
        k1 = h * rhs_with_moon(t, y);
        k2 = h * rhs_with_moon(t + h / 2.0, y + k1 / 2.0);
        k3 = h * rhs_with_moon(t + h / 2.0, y + k2 / 2.0);
        k4 = h * rhs_with_moon(t + h, y + k3);

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

    // Solve using Runge-Kutta 4th order method with Moon's gravitational effect
    valarray<double> y_rk4_with_moon = rungeKutta4(t0, y0, h, tf);

    return 0;
}
