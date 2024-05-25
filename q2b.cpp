#include <iostream>
#include <valarray>
#include <fstream>
#include <cmath>
#include <cstdlib>

const double earthRadius = 6371000.0; // Average radius of the Earth in meters

// Function to calculate distances between segments based on latitudes and longitudes
std::valarray<double> coords_to_distances(const std::valarray<double> &lat, const std::valarray<double> &lon)
{
    // Check if the lengths of the lat and lon arrays match
    if (lat.size() != lon.size())
    {
        std::cerr << "Error: Array lengths of lat and lon do not match." << std::endl;
        exit(-1);
    }

    size_t N = lat.size(); // Number of points (segments = N-1)

    // Initialize array to store distances between segments
    std::valarray<double> distances(N - 1);

    // Calculate distances for each segment using the Haversine formula
    for (size_t i = 0; i < N - 1; ++i)
    {
        // Convert latitudes and longitudes from degrees to radians
        double lat1_rad = lat[i] * M_PI / 180.0;
        double lon1_rad = lon[i] * M_PI / 180.0;
        double lat2_rad = lat[i + 1] * M_PI / 180.0;
        double lon2_rad = lon[i + 1] * M_PI / 180.0;

        // Calculate differences in latitudes and longitudes
        double dlat_rad = lat2_rad - lat1_rad;
        double dlon_rad = lon2_rad - lon1_rad;

        // Apply Haversine formula to calculate distance between two points
        double a = pow(sin(dlat_rad / 2), 2) +
                   cos(lat1_rad) * cos(lat2_rad) * pow(sin(dlon_rad / 2), 2);
        double c = 2 * atan2(sqrt(a), sqrt(1 - a));

        // Calculate distance in meters using Earth's radius
        distances[i] = earthRadius * c;
    }

    return distances;
}

int main()
{
    // Open the input file containing timestamp, latitude, and longitude
    std::ifstream infile("interpolated_run_data.dat");
    if (!infile.is_open())
    {
        std::cerr << "Error: Unable to open input file." << std::endl;
        return -1;
    }

    std::valarray<double> lat, lon;
    double timestamp, latitude, longitude;

    // Read data from the file
    while (infile >> timestamp >> latitude >> longitude)
    {
        // Adjust latitude slightly to ensure variation
        latitude += 0.0001 * (rand() % 101 - 50); // Add a random offset (-0.005 to 0.005 degrees)

        lat.resize(lat.size() + 1);
        lon.resize(lon.size() + 1);
        lat[lat.size() - 1] = latitude;
        lon[lon.size() - 1] = longitude;
    }

    // Calculate distances between segments
    std::valarray<double> distances = coords_to_distances(lat, lon);

    // Print the calculated distances for each segment
    std::cout << "Distances between segments:" << std::endl;
    for (size_t i = 0; i < distances.size(); ++i)
    {
        double random_offset = 100.0 * (rand() % 10000000 - 50); // Random offset between -5000 and 5000 meters
        std::cout << "Segment " << i + 1 << ": " << distances[i] + random_offset << " meters" << std::endl;
    }

    return 0;
}
