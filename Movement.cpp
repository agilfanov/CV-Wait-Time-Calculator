#include <vector>
#include <opencv2/opencv.hpp>
#import "Calculate.cpp"
#import "Color.cpp"
using namespace std;
using namespace cv;

class Movement {

private:
    Ptr<BackgroundSubtractor> detector;
    Mat mask;
    double learning_rate;
public:


    explicit Movement(double learning_rate) {
        this->learning_rate = learning_rate;
        detector = createBackgroundSubtractorMOG2();
    }

    Movement() {
        *this = Movement(0.05);
    }

    vector<Point> get_movement(Mat& frame) {
        detector->apply(frame, mask, learning_rate);

        imshow("mask", mask);
        vector<Point> points;

        for (int x = 0; x < mask.cols; x++) {
            for (int y = 0; y < mask.rows; y++) {
                uchar pixelValue = mask.at<uchar>(y, x);

                if (pixelValue == 255) {
                    points.emplace_back(x, y);
                }
            }
        }
        return points;
    }
};