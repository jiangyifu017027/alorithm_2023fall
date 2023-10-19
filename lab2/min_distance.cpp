#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <chrono>

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

// 按照x坐标从小到大排序
bool compareX(const Point& p1, const Point& p2) {
    return p1.x < p2.x;
}

// 按照y坐标从小到大排序
bool compareY(const Point& p1, const Point& p2) {
    return p1.y < p2.y;
}

// 递归函数，用于求解最近点对
std::pair<int, int> closestPair(const std::vector<Point>& points, int start, int end) {
    if (end - start <= 2) {
        // 当区域中只有一个或两个点时，直接返回这些点
        return { points[start].id, points[end].id };
    }

    int mid = (start + end) / 2;
    double midX = points[mid].x;

    // 递归求解左右子区域的最近点对
    std::pair<int, int> leftPair = closestPair(points, start, mid);
    std::pair<int, int> rightPair = closestPair(points, mid + 1, end);

    // 取得左右子区域的最近点对的距离
    double minDistance = std::min(distance(points[leftPair.first], points[leftPair.second]),
                                distance(points[rightPair.first], points[rightPair.second]));

    // 合并左右子区域的点，并按照y坐标从小到大排序
    std::vector<Point> mergedPoints;
    for (int i = start; i <= end; i++) {
        if (std::abs(points[i].x - midX) < minDistance) {
            mergedPoints.push_back(points[i]);
        }
    }
    std::sort(mergedPoints.begin(), mergedPoints.end(), compareY);

    // 在合并后的点集中查找更近的点对
    int size = mergedPoints.size();
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size && mergedPoints[j].y - mergedPoints[i].y < minDistance; j++) {
            double d = distance(mergedPoints[i], mergedPoints[j]);
            if (d < minDistance) {
                minDistance = d;
                leftPair = { mergedPoints[i].id, mergedPoints[j].id };
            }
        }
    }

    return leftPair;
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

    // 按照x坐标从小到大排序
    std::vector<Point> newPoints = points;
    std::sort(points.begin(), points.end(), compareX);

    // 调用分治算法求解最近点对
    std::pair<int, int> closest = closestPair(points, 0, n - 1);

    std::cout << "Closest pair: " << closest.first << " and " << closest.second << std::endl;
    std::cout << "Distance: " << distance(newPoints[closest.first], newPoints[closest.second]) << std::endl;

    // 获取结束时间点
    auto end = std::chrono::high_resolution_clock::now();

    // 计算执行时间
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // 输出执行时间（以毫秒为单位）
    std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;

    return 0;
}