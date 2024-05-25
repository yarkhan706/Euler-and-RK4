#include "q3-4.hpp"
#include <cmath>
#include <iostream>

// Define the satellite positions and measured times
std::array<double, 2> xA = {1000.0, 2000.0}; // Example values for SatA's position
std::array<double, 2> xB = {3000.0, 4000.0}; // Example values for SatB's position
double tA = 0.00001; // Example measured time for SatA
double tB = 0.000015; // Example measured time for SatB

void f(std::array<double, 2>& result, const std::array<double, 2>& x) {
    double xO = x[0];
    double yO = x[1];
    double xAx = xA[0];
    double yAy = xA[1];
    double xBx = xB[0];
    double yBy = xB[1];

    double lOA = tA * c;
    double lOB = tB * c;

    result[0] = pow(xO - xAx, 2) + pow(yO - yAy, 2) - pow(lOA, 2);
    result[1] = pow(xO - xBx, 2) + pow(yO - yBy, 2) - pow(lOB, 2);
}

void f_Jac(std::array<std::array<double, 2>, 2>& jacobian, const std::array<double, 2>& x) {
    double xO = x[0];
    double yO = x[1];
    double xAx = xA[0];
    double yAy = xA[1];
    double xBx = xB[0];
    double yBy = xB[1];

    jacobian[0][0] = 2 * (xO - xAx);
    jacobian[0][1] = 2 * (yO - yAy);
    jacobian[1][0] = 2 * (xO - xBx);
    jacobian[1][1] = 2 * (yO - yBy);
}

int main() {
    // Example usage
    std::array<double, 2> x = {1500.0, 2500.0}; // Example initial guess for (xO, yO)
    std::array<double, 2> result;
    std::array<std::array<double, 2>, 2> jacobian;

    // Evaluate the function f at the point x
    f(result, x);
    std::cout << "f(x) = [" << result[0] << ", " << result[1] << "]" << std::endl;

    // Evaluate the Jacobian f_Jac at the point x
    f_Jac(jacobian, x);
    std::cout << "Jacobian matrix:" << std::endl;
    std::cout << "[" << jacobian[0][0] << ", " << jacobian[0][1] << "]" << std::endl;
    std::cout << "[" << jacobian[1][0] << ", " << jacobian[1][1] << "]" << std::endl;

    return 0;
}
