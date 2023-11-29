#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>
using namespace std;

// 图的邻接表表示
class Graph {
private:
    unordered_map<long long, vector<long long>> adjacencyList; // 邻接表

public:
    void addEdge(long long src, long long dest) {
        adjacencyList[src].push_back(dest);
    }

    long long breadthFirstSearch(long long startVertex, long long numVertices) {
        vector<bool> visited(numVertices, false);
        queue<long long> bfsQueue;

        visited[startVertex] = true;
        bfsQueue.push(startVertex);
        long long totalVisited = 1;

        while (!bfsQueue.empty()) {
            long long currentVertex = bfsQueue.front();
            bfsQueue.pop();

            for (long long neighbor : adjacencyList[currentVertex]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    bfsQueue.push(neighbor);
                    totalVisited++;
                }
            }
        }

        return totalVisited;
    }
};

int main() {
    ifstream inputFile("twitter_small.txt");
    if (!inputFile) {
        cerr << "Failed to open twitter_small.txt" << endl;
        return 1;
    }

    long long numVertices = 1000000000;

    Graph graph;

    // 读取边并构建图
    long long src, dest;
    while (inputFile >> src >> dest) {
        graph.addEdge(src, dest);
    }

    inputFile.close();

    long long startVertex = 54226675; // 选择起始顶点

    long long totalVisited = graph.breadthFirstSearch(startVertex, numVertices);
    cout << "Total visited vertices: " << totalVisited << endl;

    return 0;
}