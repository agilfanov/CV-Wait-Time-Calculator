#include <vector>
#include <opencv2/opencv.hpp>
#include "Calculate.cpp"
#include "Color.cpp"
using namespace std;
using namespace cv;

class DBSCAN {

private:

    const int NOISE = 1e9;
    const int UNDEFINED = -1;

    vector<PointID> points;
    vector<vector<int>> adj;
    double epsilon;
    int min_neighbors;
    vector<int> cluster;
    vector<vector<int>> by_cluster;
    int cluster_count;


    void set_up_neighbors() {
        for (int id = 0; id < points.size(); id++) {
            for (int j_id = id + 1; j_id < points.size(); j_id++) {
                if (Calculate::distance(points[id], points[j_id]) <= epsilon) {
                    adj[id].push_back(j_id);
                    adj[j_id].push_back(id);
                }
            }
        }
    }

    void set_cluster(int id, int cluster_id) {
        if (cluster[id] == UNDEFINED || cluster[id] == NOISE) {
            cluster[id] = cluster_id;
            by_cluster[cluster_id].push_back(id);
        }
    }

public:

    DBSCAN(vector<Point>& points, double epsilon, int min_neighbors) {
        this->epsilon = epsilon;
        this->min_neighbors = min_neighbors;

        this->points.assign(points.size(), {0, 0, 0});
        this->cluster.assign(points.size(), UNDEFINED);
        adj.assign(points.size(), {});
        for (int id = 0; id < points.size(); id++) {
            this->points[id] = {points[id].x, points[id].y, id};
        }

        set_up_neighbors();
        cluster_count = 0;
    }

    void run_dbscan() {
        cluster_count = 0;
        by_cluster.assign(points.size() + 1, {});
        for (auto& point: points) {
            if (cluster[point.id] != UNDEFINED) continue;

            if (adj[point.id].size() < min_neighbors) {
                cluster[point.id] = NOISE;
                continue;
            }

            cluster_count++;
            set_cluster(point.id, cluster_count);

            queue<int> adj_set;
            for (int id: adj[point.id]) adj_set.push(id);
            while (!adj_set.empty()) {
                int check = adj_set.front();
                adj_set.pop();
                if (cluster[check] == NOISE) {
                    set_cluster(check, cluster_count);
                }
                if (cluster[check] != UNDEFINED) continue;

                set_cluster(check, cluster_count);

                if (adj[check].size() >= min_neighbors) {
                    for (auto& i: adj[check]) adj_set.push(i);
                }
            }
        }
    }

    int get_cluster_count() const {return cluster_count;}

    vector<Point> draw_clusters(Mat& image, int min_cluster_area) {
        vector<Point> means;
        for (int i = 1; i <= cluster_count; i++) {

            if (by_cluster[i].empty()) continue;

            // draw the stuff of this cluster
            vector<Point> cluster_members;
            Point mean;

            for (int id: by_cluster[i]) {
                cluster_members.emplace_back(points[id].x, points[id].y);
                mean.x += points[id].x;
                mean.y += points[id].y;
            }


            pair<double, vector<Point>> hull = Calculate::convex_hull_area(cluster_members);
            if (hull.first >= min_cluster_area) {

                circle(image, mean, 10, colors[i % colors.size()], 10);
                means.emplace_back((int)((double)mean.x / (double)by_cluster[i].size()), (int)((double)mean.y / (double)by_cluster[i].size()));
                for (int j = 1; j < hull.second.size(); j++) {
                    line(image, hull.second[j - 1], hull.second[j], colors[i % colors.size()], 3);
                }
            }
        }
        return means;
    }
};