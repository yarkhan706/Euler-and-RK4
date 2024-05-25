#include <iostream>
#include <fstream>
#include <sstream>
#include <valarray>
#include <cmath> // Include cmath for mathematical functions like std::sqrt
#include <string>
#include <stdexcept>

const double earthRadius = 6371000.0; // Radius of the Earth in meters

// Function to read GPS data from a file and return a valarray
std::valarray<double> read_gps_data(const std::string &filename, int N)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return {}; // Return empty valarray on error
    }

    std::valarray<double> data(N * 4); // Each data point has four values

    std::string line;
    int index = 0;
    while (std::getline(file, line) && index < N)
    {
        std::stringstream ss(line);
        for (int i = 0; i < 4; ++i)
        {
            std::string cell;
            if (!std::getline(ss, cell, ' '))
            {
                std::cerr << "Error: Insufficient data in file " << filename << std::endl;
                return {}; // Return empty valarray on error
            }
            try
            {
                data[index * 4 + i] = std::stod(cell);
            }
            catch (const std::invalid_argument &e)
            {
                std::cerr << "Error: Invalid data in file " << filename << " at row " << index << std::endl;
                return {}; // Return empty valarray on error
            }
        }
        ++index;
    }

    if (index < N)
    {
        std::cerr << "Error: File does not contain enough data points." << std::endl;
        return {}; // Return empty valarray on error
    }

    return data;
}

// Function to calculate distances between GPS coordinates
std::valarray<double> coords_to_distances(const std::valarray<double> &lat, const std::valarray<double> &lon)
{
    if (lat.size() != lon.size())
    {
        std::cerr << "Error: Input arrays (lat, lon) must have the same size." << std::endl;
        std::exit(-1);
    }

    size_t N = lat.size() - 1;
    std::valarray<double> dlat = lat[std::slice(1, N, 1)] - lat[std::slice(0, N, 1)];
    std::valarray<double> dlon = lon[std::slice(1, N, 1)] - lon[std::slice(0, N, 1)];

    std::valarray<double> dl(N);
    for (size_t i = 0; i < N; ++i)
    {
        double dlat2 = dlat[i] * dlat[i];
        double dlon2 = dlon[i] * dlon[i];
        double cos_lat = std::cos(lat[i] * M_PI / 180.0);

        dl[i] = earthRadius * std::sqrt(dlat2 + cos_lat * cos_lat * dlon2);
    }

    return dl;
}

// Function to calculate speeds from distances
std::valarray<double> dl_to_speed(const std::valarray<double>& dl) {
    size_t N = dl.size();
    std::valarray<double> speed(N);

    // Calculate speed using centered difference for each segment
    for (size_t i = 1; i < N; ++i) {
        speed[i] = dl[i - 1] / 1.0; // Assuming data is sampled every 1 second (dt = 1)
    }

    return speed;
}


class Run
{
public:
    double t_start;                 // Starting time of the run (Unix timestamp format, in seconds)
    double t_end;                   // End time of the run (Unix timestamp format, in seconds)
    int duration;                   // Duration of the run in seconds (excluding pauses)
    double distance;                // Total distance covered in meters
    std::valarray<double> lat_data; // Latitude data corresponding to timestamps
    std::valarray<double> lon_data; // Longitude data corresponding to timestamps

    // Constructor with initialization
    Run(double start_time, const std::valarray<double> &latitudes, const std::valarray<double> &longitudes)
        : t_start(start_time), t_end(0.0), duration(0), distance(0.0), lat_data(latitudes), lon_data(longitudes) {}

    // Function to end the run and calculate duration and distance
    void endRun(double end_time, double total_distance)
    {
        t_end = end_time;
        duration = static_cast<int>(t_end - t_start);
        distance = total_distance;
    }

    // Function to calculate average pace
    double get_avg_pace() const
    {
        std::valarray<double> dl = coords_to_distances(lat_data, lon_data);
        std::valarray<double> speed = dl_to_speed(dl);

        //duration > 0 && distance > 0 && speed.size() > 0
        if (distance > 0 && speed.size() > 0)
        {
            double avg_speed = speed.sum() / speed.size();
            double avg_pace = 60.0 / (avg_speed / 1000.0); // Average pace in minutes per kilometer
            return avg_pace;
        }
        else
        {
            return 0.0;
        }
    }

    double get_fastest_pace() const
    {
        std::valarray<double> dl = coords_to_distances(lat_data, lon_data);
        std::valarray<double> speed = dl_to_speed(dl);

        //prints speed and distance

        if (speed.size() > 0 && distance > 0)
        {
            double fastest_speed = speed.size();
            if (fastest_speed > 0)
            {
                double fastest_pace = 60.0 / (fastest_speed / 1000.0); // Fastest pace in minutes per kilometer
                return fastest_pace;
            }
            else
            {
                return 0.0; // Handle zero or negative speed case
            }
        }
        else
        {
            return 0.0;
        }
    }
};

// int main() {
//     const std::string filename = "interpolated_coordinates.dat";
//     const int Ndata = 30; // Number of data points from previous interpolation

//     // Read interpolated GPS data from file
//     std::valarray<double> data = read_gps_data(filename, Ndata);

//     // Check if data was successfully read
//     if (data.size() == 0) {
//         std::cerr << "Error: No data read from file or invalid data." << std::endl;
//         return 1; // Exit with error
//     }

//     // Print the retrieved data
//     std::cout << "Retrieved GPS Data:" << std::endl;
//     for (size_t i = 0; i < Ndata; ++i) {
//         int index = static_cast<int>(data[i * 4]);
//         double timestamp = data[i * 4 + 1];
//         double latitude = data[i * 4 + 2];
//         double longitude = data[i * 4 + 3];
//         std::cout << "Index: " << index << ", Timestamp: " << timestamp
//                   << ", Latitude: " << latitude << ", Longitude: " << longitude << std::endl;
//     }

//     // Extract timestamp, latitude, and longitude arrays
//     std::valarray<double> t_varr(Ndata), lat_varr(Ndata), lon_varr(Ndata);
//     for (int i = 0; i < Ndata; ++i) {
//         t_varr[i] = data[i * 4 + 1]; // Timestamp
//         lat_varr[i] = data[i * 4 + 2]; // Latitude
//         lon_varr[i] = data[i * 4 + 3]; // Longitude
//     }

//     // Create a Run instance
//     Run myRun(t_varr[0], lat_varr, lon_varr);

//     // Simulate the run end (example: end at timestamp 1711960000.0 with distance 1000.0 meters)
//     myRun.endRun(t_varr[Ndata - 1], 1000.0);

//     // Calculate and print average pace
//     double avgPace = myRun.get_avg_pace();

//     std::cout << "Average Pace: " << avgPace << " min/km" << std::endl;

//     // Calculate and print fastest pace
//     double fastestPace = myRun.get_fastest_pace();
//     std::cout << "Fastest Pace: " << fastestPace << " min/km" << std::endl;

//     return 0;
// }
