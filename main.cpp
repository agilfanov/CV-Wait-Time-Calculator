#import "DBSCAN.cpp"
#import "Calculate.cpp"
#import "Movement.cpp"
#import "Vision.cpp"
#import "Line.cpp"
using namespace std;


int main() {

    Line line (1, 2, 17, 10, 0.05, 45, 30, 5, 5, 100, {1920, 1080}, 0, 96);

    line.run_line();

    cout << line.get_wait_minutes() << endl;
    cout << line.get_people_in_line() << endl;
    return 0;
}

