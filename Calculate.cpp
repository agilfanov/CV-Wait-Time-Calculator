#include <vector>
#include "PointID.cpp"
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

class Calculate {
public:

    static double distance(PointID& a, PointID& b) {
        return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    }
    static double distance(Point& a, Point& b) {
        return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    }
    static double triangle_area(Point& a, Point& b, Point& c) {
        return 0.5 * abs(a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
    }

    static bool point_in_rect(Point& a, Rect& r) {
        return (a.x >= r.x && a.x <= r.x + r.width && a.y >= r.y && a.y <= r.y + r.height);
    }

    static Rect get_bounding_rect(Point& a, Point& b, int extra) {
        return {min(a.x, b.x) - extra, min(a.y, b.y) - extra, max(a.x, b.x) - min(a.x, b.x) + 2 * extra, min(a.y, b.y) - min(a.y, b.y) + 2 * extra};
    }

    static int get_hue(Mat& image, int x, int y) {
        return image.at<Vec3b>(y, x)[0];
    }

    static int get_saturation(Mat& image, int x, int y) {
        return image.at<Vec3b>(y, x)[1];
    }

    static int get_value(Mat& image, int x, int y) {
        return image.at<Vec3b>(y, x)[2];
    }

    static pair<double, vector<Point>> convex_hull_area(vector<Point>& points) {

        vector<Point> hull {};
        convexHull(points, hull);

        double area = 0;

        for (int i = 2; i < points.size(); i++) {
            area += triangle_area(points[0], points[i - 1], points[i]);
        }
        return {area, hull};
    }

};