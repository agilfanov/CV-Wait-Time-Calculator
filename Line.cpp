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
    int people_per_cycle;
    int cycle_time;
    int camera_index;
    Vision camera;


public:

    Line(int people_per_cycle, int cycle_time, int epsilon, int min_neighbors, double learning_rate, int x_space, int y_space, int banned_time, int banned_extra_space, int min_object_size, Size native_camera_resolution, int camera_index, int threshold) {
        this->people_per_cycle = people_per_cycle;
        this-> cycle_time = cycle_time;
        this->camera_index = camera_index;
        camera = Vision(epsilon, min_neighbors, learning_rate, x_space, y_space, banned_time, banned_extra_space, min_object_size, native_camera_resolution, threshold);
    }


    void run_line() {
        camera.start_camera(camera_index);
        while (waitKey(1) != 'q') {
            string text = "Current Wait Time: " + to_string(this->get_wait_minutes());
            camera.run_one_frame(text);
        }
    }

    int get_people_in_line() {
        return camera.get_people_left_of_line();
    }

    int get_wait_minutes() {

        int people = (int)camera.get_people_left_of_line();

        int cycle_in = people / people_per_cycle;

        return cycle_in * cycle_time;
    }

};