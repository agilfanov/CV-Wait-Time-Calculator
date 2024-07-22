#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <opencv2/opencv.hpp>
#import "DBSCAN.cpp"
#import "Calculate.cpp"
#import "Movement.cpp"

using namespace std;
using namespace cv;


class Vision {

private:

    VideoCapture camera;

    vector<Point> prevFrame;
    deque<pair<int, Rect>> banned;
    Movement movement_operator;
    int people_left_of_line = 0;
    Size resolution;

    int epsilon;
    int min_neighbors;
    double learning_rate;
    int x_space;
    int y_space;
    int banned_time;
    int banned_extra_space;
    int clock;
    int threshold;
    int min_object_size;

public:


    Vision(int epsilon, int min_neighbors, double learning_rate, int x_space, int y_space, int banned_time, int banned_extra_space, int min_object_size, Size native_camera_resolution, int threshold) {
        this->epsilon = epsilon;
        this->min_neighbors = min_neighbors;
        this->learning_rate = learning_rate;
        this->x_space = x_space;
        this->y_space = y_space;
        this->banned_time = banned_time;
        this->banned_extra_space = banned_extra_space;
        this->min_object_size = min_object_size;
        this->threshold = threshold;
        double ratio = (double)native_camera_resolution.width / native_camera_resolution.height;
        resolution.width = 192;
        resolution.height = (int)((double)192 / ratio);
        movement_operator = Movement(learning_rate);
    }

    Vision() {
        *this = Vision(10, 4, 0.05, 5, 5, 5, 5, 300, {1920, 1080}, 96);
    }


    void start_camera(int camera_index) {
        people_left_of_line = 0;
        camera = VideoCapture(camera_index);
        prevFrame.assign({});
        banned.assign({});
        Movement movement (learning_rate);
        clock = 0;

    }


    void run_one_frame(string& text) {
        clock++;
        Mat frame;
        camera >> frame;
        resize(frame, frame, resolution);
        line(frame, {threshold, 0}, {threshold, frame.rows - 1}, Scalar(255, 255, 255), 2);



        vector<Point> points = movement_operator.get_movement(frame);

        while (!banned.empty() && clock - (banned.front().first) >= banned_time) {
            banned.pop_front();
        }

        DBSCAN db(points, epsilon, min_neighbors);

        db.run_dbscan();

        vector<Point> curr_frame = db.draw_clusters(frame, min_object_size);


        for (auto curr: curr_frame) {
            for (auto prev: prevFrame) {
                if (abs(prev.x - curr.x) <= x_space && abs(prev.y - curr.y) <= y_space) {
                    bool ok = true;
                    for (auto& p: banned) {
                        Rect r = p.second;
                        if (Calculate::point_in_rect(curr, r) && Calculate::point_in_rect(prev, r)) {
                            ok = false;
                            break;
                        }
                    }
                    if (!ok) continue;
                    if (curr.x > threshold && prev.x < threshold) {
                        people_left_of_line--;
                        banned.emplace_back(clock, Calculate::get_bounding_rect(curr, prev, banned_extra_space));
                    } else if (curr.x < threshold && prev.x > threshold){
                        people_left_of_line++;
                        banned.emplace_back(clock, Calculate::get_bounding_rect(curr, prev, banned_extra_space));
                    }
                }
            }
        }
        prevFrame = curr_frame;

        people_left_of_line = max(people_left_of_line, 0);
        putText(frame, "Left of Line: " + to_string(people_left_of_line), {0, 10}, FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 0), 1);
        putText(frame, text, {0, 25}, FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 0), 1);

        imshow("Frame", frame);
    }

    int get_people_left_of_line() {return people_left_of_line;}

};
