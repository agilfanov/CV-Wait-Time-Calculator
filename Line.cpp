#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <opencv2/opencv.hpp>
#import "DBSCAN.cpp"
#import "Calculate.cpp"
#import "Movement.cpp"
#import "Vision.cpp"

using namespace std;
using namespace cv;

class Line {
private:
    double people_per_minute;
    Vision camera ;


public:

    Line(double people_per_minute, int epsilon, int min_neighbors, double learning_rate, int x_space, int y_space, int banned_time, int banned_extra_space, int min_object_size, Size native_camera_resolution) {
        this->people_per_minute = people_per_minute;
        camera = Vision(epsilon, min_neighbors, learning_rate, x_space, y_space, banned_time, banned_extra_space, min_object_size, native_camera_resolution);
    }

    Line(int people_per_cycle, int cycle_time, int epsilon, int min_neighbors, double learning_rate, int x_space, int y_space, int banned_time, int banned_extra_space, int min_object_size, Size native_camera_resolution) {
       *this = Line((double)people_per_cycle / cycle_time, epsilon, min_neighbors, learning_rate, x_space, y_space, banned_time, banned_extra_space, min_object_size, native_camera_resolution);
    }

    void run_line() {
        camera.start_camera(0);
        while (waitKey(1) != 'q') {
            camera.run_one_frame("Current Wait Time: " + to_string(this->get_wait_minutes()));
        }
    }

    int get_wait_minutes() {
        double time = (double)camera.get_people_left_of_line() / people_per_minute;
        return (int)time;
    }

};