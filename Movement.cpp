#include <vector>
#include <opencv2/opencv.hpp>
#include "Calculate.cpp"
#include "Color.cpp"
using namespace std;
using namespace cv;

class Movement {

    // could make it myself to maybe better be vs change with like clouds or just prolly use the     Ptr<BackgroundSubtractor> pMOG2 = createBackgroundSubtractorMOG2();

    // do in HSV, H and S values matter more, V is brigthness which we can get avg brightness change of whole frame, and like not care about it as much if whole frame changes



private:
    int history_size;
    double change_threshold;
    deque<Mat> history;


    int get_avg_value(Mat& image) {
        int sum = 0;
        for (int x = 0; x < image.cols; x++) {
            for (int y = 0; y < image.rows; y++) {
                sum += Calculate::get_value(image, x, y);
            }
        }
        return (int)((double)sum / (double)(image.cols * image.rows));
    }

public:

    Movement(int history_size, double change_threshold) {
        this->history_size = history_size;
        this->change_threshold = change_threshold;
    }

    void feed_frame(Mat& frame) {
        cvtColor(frame, frame, COLOR_BGR2HSV);

        history.push_back(frame);
        while (history.size() > history_size) {
            history.pop_front();
        }

    }

    vector<Point> detect_movement() {
        vector<Point> ret;
        Mat latest = history[history.size() - 1];
        for (int x = 0; x < latest.cols; x++) {
            for (int y = 0; y < latest.rows; y++) {
                
            }
        }


        return ret;
    }


};