#include <iostream>
#include <cmath>
#include <valarray>
#include "q3-4.hpp"

using namespace std;

/* Use these functions to print 2-vectors and 2x2 matrices */

void print_mat(double M[2][2]) {
    cout << M[0][0] << ' ' << M[0][1] << endl << M[1][0] << ' ' << M[1][1] << endl;
}

void print_vec(double v[2]) {
    cout << v[0] << ' ' << v[1] << endl;
}

/* Function to calculate the inverse of a 2x2 matrix */
void mat_inv_2x2(double J[2][2], double J_inv[2][2]) {
    double det = J[0][0] * J[1][1] - J[0][1] * J[1][0];
    if (det == 0) {
        cerr << "Error: Matrix is singular, cannot invert." << endl;
        return;
    }

    J_inv[0][0] = J[1][1] / det;
    J_inv[0][1] = -J[0][1] / det;
    J_inv[1][0] = -J[1][0] / det;
    J_inv[1][1] = J[0][0] / det;
}

/* Implement the system of equations function f and its Jacobian function f_Jac */
void f(double result[2], const double x[2]) {
    // Define the equations here based on the problem description
    result[0] = pow(x[0], 2) - 2 * x[0] * x[1] + pow(x[1], 2) - 1;
    result[1] = x[0] + pow(x[1], 2) - 4;
}

void f_Jac(double jacobian[2][2], const double x[2]) {
    // Calculate the Jacobian matrix here based on the partial derivatives
    jacobian[0][0] = 2 * x[0] - 2 * x[1];
    jacobian[0][1] = -2 * x[0] + 2 * x[1];
    jacobian[1][0] = 1;
    jacobian[1][1] = 2 * x[1];
}

/* Implement the 2D Newton-Raphson method for solving a system of equations */
int newton_raphson(double xi[2], const double x0[2], int Nmax, double TOL) {
    double x[2], dx[2], fx[2], J[2][2], J_inv[2][2];
    int i = 0;

    // Initialize x with the initial guess x0
    x[0] = x0[0];
    x[1] = x0[1];

    do {
        // Evaluate the function values and Jacobian at the current point x
        f(fx, x);
        f_Jac(J, x);

        // Calculate the inverse of the Jacobian matrix
        mat_inv_2x2(J, J_inv);

        // Compute the Newton-Raphson update
        dx[0] = -J_inv[0][0] * fx[0] - J_inv[0][1] * fx[1];
        dx[1] = -J_inv[1][0] * fx[0] - J_inv[1][1] * fx[1];

        // Update x
        x[0] += dx[0];
        x[1] += dx[1];

        // Increment iteration count
        i++;

        // Check convergence criteria
    } while ((fabs(dx[0] / x[0]) > TOL || fabs(dx[1] / x[1]) > TOL) && i <= Nmax);

    // Store the final solution in xi
    xi[0] = x[0];
    xi[1] = x[1];

    // Output the iteration count and convergence status
    if (i > Nmax) {
        cout << "Newton-Raphson did not converge after " << Nmax << " iterations." << endl;
        return -1;
    } else {
        cout << "Newton-Raphson converged after " << i << " iterations." << endl;
        return 0;
    }
}

int main() {
    // Define parameters for Newton-Raphson method
    double x0[2] = {1.0, 1.0}; // Initial guess
    int Nmax = 100;             // Maximum number of iterations
    double TOL = 1e-6;          // Tolerance for convergence

    // Array to store the solution
    double xi[2];

    // Call the Newton-Raphson method
    int status = newton_raphson(xi, x0, Nmax, TOL);

    // Print the solution
    if (status == 0) {
        cout << "Solution found: ";
        print_vec(xi);
    }

    return 0;
}
