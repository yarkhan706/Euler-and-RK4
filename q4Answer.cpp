#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

void printLastLineDetails(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string lastLine, line;
    while (getline(file, line)) {
        lastLine = line;
    }

    stringstream ss(lastLine);
    int N;
    double Rx, Ry, Vx, Vy;
    string TF;

    // Extract values from the last line
    ss >> TF >> Rx >> Ry >> Vx >> Vy >> N;

    // Print details
    cout << filename << endl;
    cout << "TF is " << TF << " rx = " << Rx << " ry = " << Ry << " vx = " << Vx << " vy =  " << Vy << endl;

    file.close();
}

int main() {
    // Print details of last line of euler_output.dat
    printLastLineDetails("euler_output.dat");
    printLastLineDetails("euler_output86.dat");
    printLastLineDetails("rk4_output.dat");
    printLastLineDetails("rk4_output86.dat");

    return 0;
}
