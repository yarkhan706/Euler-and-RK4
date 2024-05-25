#include <iostream>
#include <fstream>
#include <sstream>
#include <valarray>
#include <cmath>
#include <string>
#include <stdexcept>
#include "run.cpp" // Include the Run class implementation

class Runner {
private:
    std::string username;
    std::valarray<double> latitudes;
    std::valarray<double> longitudes;

public:
    Runner(const std::string& name) : username(name) {}

    void readGPSData(const std::string& filename, int N) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file " << filename << std::endl;
            return;
        }

        std::valarray<double> data(N * 4);
        std::string line;
        int index = 0;

        while (std::getline(file, line) && index < N) {
            std::stringstream ss(line);
            for (int i = 0; i < 4; ++i) {
                std::string cell;
                if (!std::getline(ss, cell, ' ')) {
                    std::cerr << "Error: Insufficient data in file " << filename << std::endl;
                    return;
                }
                try {
                    data[index * 4 + i] = std::stod(cell);
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Error: Invalid data in file " << filename << " at row " << index << std::endl;
                    return;
                }
            }
            ++index;
        }

        if (index < N) {
            std::cerr << "Error: File does not contain enough data points." << std::endl;
            return;
        }

        // Extract latitude and longitude arrays
        latitudes.resize(N);
        longitudes.resize(N);
        for (int i = 0; i < N; ++i) {
            latitudes[i] = data[i * 4 + 2]; // Latitude
            longitudes[i] = data[i * 4 + 3]; // Longitude
        }
    }

    void simulateRun(double start_time, double end_time, double total_distance) {
        Run myRun(start_time, latitudes, longitudes);
        myRun.endRun(end_time, total_distance);

        // Calculate and print average pace
        double avgPace = myRun.get_avg_pace();
        std::cout << "Average Pace for " << username << ": " << avgPace << " min/km" << std::endl;

        // Calculate and print fastest pace
        double fastestPace = myRun.get_fastest_pace();
        std::cout << "Fastest Pace for " << username << ": " << fastestPace << " min/km" << std::endl;
    }
};

int main() {
    const std::string filename = "interpolated_coordinates.dat";
    const int Ndata = 30; // Number of data points from previous interpolation

    // Create a Runner instance
    Runner myRunner("JohnDoe");

    // Read interpolated GPS data from file
    myRunner.readGPSData(filename, Ndata);

    // Simulate the run (example: start at timestamp 1000.0, end at timestamp 2000.0 with distance 1000.0 meters)
    myRunner.simulateRun(1000.0, 2000.0, 1000.0);

    return 0;
}
