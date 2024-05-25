#include <iostream>
#include <iomanip>
#include <valarray>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "interp.cpp"

using namespace std;

/* Read a file with GPS data formatted in 3 space-separated columns:
 * timestamp latitude longitude
 * and return the numerical data in the form of a single
 * valarray of size N*3. The data is read line-by line,
 * so that the value of row i and column j is stored in
 * the (3*i + j)-th component of the valarray
 */
valarray<double> read_gps_data(const string &filename, int N)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Unable to open file " << filename << endl;
        return valarray<double>(); // Return empty valarray on error
    }

    valarray<double> data(N * 3);

    string line;
    int row = 0;
    while (getline(file, line) && row < N)
    {
        stringstream ss(line);
        string cell;
        int col = 0;
        while (getline(ss, cell, ' ') && col < 3)
        {
            try
            {
                data[row * 3 + col] = stod(cell);
            }
            catch (const invalid_argument &e)
            {
                cerr << "Error: Invalid data in file " << filename << " at row " << row << endl;
                return valarray<double>(); // Return empty valarray on error
            }
            ++col;
        }
        ++row;
    }
    return data;
}

/* Function to calculate the distance between two latitude/longitude points using the Haversine formula */
double haversine(double lat1, double lon1, double lat2, double lon2)
{
    const double R = 6371e3; // Earth's radius in meters
    double phi1 = lat1 * M_PI / 180.0;
    double phi2 = lat2 * M_PI / 180.0;
    double delta_phi = (lat2 - lat1) * M_PI / 180.0;
    double delta_lambda = (lon2 - lon1) * M_PI / 180.0;

    double a = sin(delta_phi / 2) * sin(delta_phi / 2) +
               cos(phi1) * cos(phi2) *
                   sin(delta_lambda / 2) * sin(delta_lambda / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return R * c; // Distance in meters
}

double calculate_avg_pace(const valarray<double> &lat_data, const valarray<double> &lon_data, double duration)
{
    double total_distance = 0.0;
    for (size_t i = 1; i < lat_data.size(); ++i)
    {
        total_distance += haversine(lat_data[i - 1], lon_data[i - 1], lat_data[i], lon_data[i]);
    }
    double total_distance_km = total_distance / 1000.0;      // Convert to kilometers
    double avg_pace = (duration / 60.0) / total_distance_km; // Pace in min/km
    return avg_pace;
}

/* Function to calculate the fastest pace in min/km over any 1 km segment */
double calculate_fastest_pace(const valarray<double> &lat_data, const valarray<double> &lon_data)
{
    double total_distance = 0.0;
    double segment_distance = 0.0;
    double fastest_pace = std::numeric_limits<double>::max();
    size_t start_index = 0;

    for (size_t i = 1; i < lat_data.size(); ++i)
    {
        segment_distance += haversine(lat_data[i - 1], lon_data[i - 1], lat_data[i], lon_data[i]);
        if (segment_distance >= 1000.0)
        {                                                                          // If segment distance is 1 km or more
            double segment_duration = i - start_index;                             // Duration in seconds
            double pace = (segment_duration / 60.0) / (segment_distance / 1000.0); // Pace in min/km
            if (pace < fastest_pace)
            {
                fastest_pace = pace;
            }
            start_index = i;        // Move start index to current position
            segment_distance = 0.0; // Reset segment distance
        }
    }
    return fastest_pace;
}

/* Function to perform piecewise quadratic Lagrange interpolation */
double interp_coords(valarray<double> &t_varr, valarray<double> &coord_varr, double t)
{
    size_t n = t_varr.size();
    if (n < 3)
    {
        cerr << "Error: Not enough data points for quadratic interpolation." << endl;
        return 0.0;
    }

    size_t i = 0;
    while (i < n - 2 && t > t_varr[i + 2])
    {
        ++i;
    }

    valarray<double> t_segment = t_varr[slice(i, 3, 1)];
    valarray<double> coord_segment = coord_varr[slice(i, 3, 1)];

    return lagrange_interp(t_segment, coord_segment, t);
}

int main(int argc, char *argv[])
{
    int Ndata = 1408;
    valarray<double> data = read_gps_data("test_run_coords.dat", Ndata);

    if (data.size() == 0)
    {
        return 1; // Exit if data reading failed
    }

    valarray<double> t_varr(Ndata), lat_varr(Ndata), lon_varr(Ndata);

    /* Taking slices of data (with stride=3) to extract the three columns individually */
    t_varr = data[slice(0, Ndata, 3)];
    lat_varr = data[slice(1, Ndata, 3)];
    lon_varr = data[slice(2, Ndata, 3)];

    double duration = t_varr.max() - t_varr.min();

    double avg_pace = calculate_avg_pace(lat_varr, lon_varr, duration);
    double fastest_pace = calculate_fastest_pace(lat_varr, lon_varr);

    cout << "Run Summary:" << endl;
    cout << "Total Duration: " << duration << " seconds" << endl;
    cout << "Average Pace: " << fixed << setprecision(2) << avg_pace << " min/km" << endl;
    cout << "Fastest Pace: " << fixed << setprecision(2) << fastest_pace << " min/km" << endl;

    // Generate a dense time grid with dt = 0.1 s
    double dt = 0.1;
    size_t dense_size = static_cast<size_t>((t_varr.max() - t_varr.min()) / dt) + 1;
    valarray<double> t_dense(dense_size);
    for (size_t i = 0; i < dense_size; ++i)
    {
        t_dense[i] = t_varr.min() + i * dt;
    }

    // Interpolate and write data to file
    ofstream output_file("interpolated_run_data.dat");
    if (!output_file.is_open())
    {
        cerr << "Error: Unable to open output file." << endl;
        return 1;
    }

    output_file << "i t lat lon" << endl;
    for (size_t i = 0; i < dense_size; ++i)
    {
        double lat = interp_coords(t_varr, lat_varr, t_dense[i]);
        double lon = interp_coords(t_varr, lon_varr, t_dense[i]);
        output_file << i << " " << t_dense[i] << " " << lat << " " << lon << endl;
    }

    output_file.close();

    return 0;
}
