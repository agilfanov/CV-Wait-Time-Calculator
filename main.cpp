#import "DBSCAN.cpp"
#import "Calculate.cpp"
#import "Movement.cpp"
#import "Vision.cpp"
#import "Line.cpp"
using namespace std;
using namespace cv;


int main() {

    Line v (10, 2, 17, 10, 0.05, 10, 10, 5, 5, 400, {1920, 1080});

    v.run_line();

    return 0;
}

