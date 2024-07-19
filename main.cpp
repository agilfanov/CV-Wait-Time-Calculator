#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <queue>
#include <stack>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "DBSCAN.cpp"

using namespace std;
using namespace cv;


int epsilon = 0;
int minN = 0;

const int maxSliderValue = 100;
const int maxSliderValue2 = 50;


void onTrackbarSlide(int pos, void* userdata) {
    epsilon = pos;
}
void onTrackbarSlide2(int pos, void* userdata) {
    minN = pos;
}


/*
 *
 *
 * This is like perfect. We take the points that are moving(aka difference from previos frame), then cluster them. That way can track moving objects in the video. What a w.
 * Only issue is like if cloud changes brightness of entire thing, then all will change by a bit.
 *
 * We can also for speed make the scale really small becuase we don't really need super high quality images.
 *
 * Make sure to also then filter by size, don't need clusters with like 20 points even if they like satisfy the other constraints
 *
 *
 * To Do:
 *
 *
 * - Add DBSCAN size filtering by convex hull area
 * - Increase DBSCAN speed --> increased a bit but still can do more.
 * - Make movement class, maybe do it by myseld?
 * - decrease resolution quite a bit to increase speed --> possibly decreasre more but right now is ok
 *
 *
 */

int main() {
    VideoCapture cap(0);


    Ptr<BackgroundSubtractor> pMOG2 = createBackgroundSubtractorMOG2();
    int time = 0;



    int inside = 0;

    int threshold = 96;
    int x_lim = 5;
    int y_lim = 5;
    set<pair<int, pair<int, int>>> q;
    int stay = 10;
    vector<Point> prevFrame;
    deque<pair<int, Rect>> banned;
    while (true) {
        Mat frame;
        cap >> frame;
        resize(frame, frame, Size(192, 108));
        line(frame, {threshold, 0}, {threshold, frame.rows - 1}, Scalar(255, 255, 255), 2);

        // Perform background subtraction
        Mat fgMask;  // Foreground mask
        pMOG2->apply(frame, fgMask, 0.05);  // play with learning rate


        imshow("Foreground Mask", fgMask);

        vector<Point> points;

        for (int x = 0; x < fgMask. cols; x++) {
            for (int y = 0; y < fgMask.rows; y++) {
                uchar pixelValue = fgMask.at<uchar>(y, x);

                if (pixelValue == 255) {
                    points.emplace_back(x, y);
                }
            }
        }

        while (!banned.empty() && time - (banned.front().first) >= stay) {
            banned.pop_front();
        }

        if (time > 10) {
            DBSCAN db(points, 19, 12);

            db.run_dbscan();

            vector<Point> curr_frame = db.draw_clusters(frame, 400);


            for (auto curr: curr_frame) {
                for (auto prev: prevFrame) {
                    if (abs(prev.x - curr.x) <= x_lim && abs(prev.y - curr.y) <= y_lim) {
                        bool ok = true;
                        for (auto& p: banned) {
                            Rect r = p.second;
                            if (Calculate::point_in_rect(curr, r) && Calculate::point_in_rect(prev, r)) {
                                ok = false;
                                break;
                            }
                        }
                        if (!ok) continue;
                        if (curr.x >= threshold && prev.x <= threshold) {
                            inside--;
                            banned.emplace_back(time, Calculate::get_bounding_rect(curr, prev, 7));
                        } else if (curr.x <= threshold && prev.x >= threshold){
                            inside++;
                            banned.emplace_back(time, Calculate::get_bounding_rect(curr, prev, 7));
                        }
                    }
                }
            }
            prevFrame = curr_frame;
//            for (auto p: curr_frame) {
//                q.insert({time, {p.x, p.y}});
//            }
        }

//        while (!q.empty() && time - (q.begin()->first) > stay) {
//            q.erase(q.begin());
//        }
        for (auto p: q) {
            circle(frame, {p.second.first, p.second.second}, 3, {255, 255, 0}, 2);
        }

        time++;
        string have = "Left of Lines: " + to_string(inside);
        putText(frame, have, {0, 10}, FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 0), 1);
        imshow("Frame", frame);
        waitKey(1);
    }
    return 0;
}

