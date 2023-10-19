#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <limits>

struct Point {
    int id;
    double x;
    double y;
};

// 计算两点之间的距离
double distance(const Point& p1, const Point& p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return std::sqrt(dx * dx + dy * dy);
}

int main() {
    std::ifstream inputFile("data.txt");
    if (!inputFile.is_open()) {
        std::cout << "Failed to open the input file." << std::endl;
        return 0;
    }

    std::vector<Point> points;
    int id;
    double x, y;
    while (inputFile >> id >> x >> y) {
        points.push_back({ id, x, y });
    }
    inputFile.close();

    int n = points.size();
    int closestPoint1 = -1;
    int closestPoint2 = -1;
    double minDistance = std::numeric_limits<double>::max();

    // 遍历所有点对，计算距离并更新最小距离和最近点对的顶点编号
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            double d = distance(points[i], points[j]);
            if (d < minDistance) {
                minDistance = d;
                closestPoint1 = points[i].id;
                closestPoint2 = points[j].id;
            }
        }
    }

    std::cout << "Closest pair: " << closestPoint1 << " and " << closestPoint2 << std::endl;
    std::cout << "Distance: " << minDistance << std::endl;

    return 0;
}