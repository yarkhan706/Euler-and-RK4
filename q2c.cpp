#include <iostream>
#include <valarray>

using namespace std;

// Function to calculate speed from displacement data
valarray<double> dl_to_speed(valarray<double>& dl) {
    valarray<double> speed(dl.size() - 1); // Speed array has one less element

    for (size_t i = 1; i < dl.size(); ++i) {
        // Calculate speed using first-order centered finite differences
        speed[i - 1] = (dl[i] - dl[i - 1]) / 1.0; // Assuming data is sampled every 1s
    }

    return speed;
}

// Function to calculate average pace
double get_avg_pace(valarray<double>& dl) {
    valarray<double> speed = dl_to_speed(dl);
    double total_distance = dl[dl.size() - 1];
    double total_time = dl.size() - 1; // Total time is number of samples - 1

    // Calculate average pace (time per unit distance)
    double avg_pace = total_time / total_distance;
    return avg_pace;
}

// Function to calculate fastest pace
double get_fastest_pace(valarray<double>& dl) {
    valarray<double> speed = dl_to_speed(dl);
    double fastest_pace = speed.min(); // Fastest pace is the minimum speed
    return 1.0 / fastest_pace; // Convert speed to pace (time per unit distance)
}

int main() {
    // Example usage
    valarray<double> dl = {0.0, 10.0, 20.0, 30.0, 40.0}; // Displacement data
    double avg_pace = get_avg_pace(dl);
    double fastest_pace = get_fastest_pace(dl);

    cout << "Average pace: " << avg_pace << " s/m" << endl;
    cout << "Fastest pace: " << fastest_pace << " s/m" << endl;

    return 0;
}
