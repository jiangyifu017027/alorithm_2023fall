#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
using namespace std;

// 图的邻接矩阵表示
class Graph {
private:
    int numVertices; // 节点数
    vector<vector<bool>> adjacencyMatrix; // 邻接矩阵

public:
    Graph(int numVertices) {
        this->numVertices = numVertices;
        adjacencyMatrix.resize(numVertices, vector<bool>(numVertices, false));
    }

    void addEdge(int src, int dest) {
        adjacencyMatrix[src][dest] = true;
        adjacencyMatrix[dest][src] = true;
    }

    void breadthFirstSearch(int startVertex) {
        vector<bool> visited(numVertices, false);
        queue<int> bfsQueue;

        visited[startVertex] = true;
        bfsQueue.push(startVertex);

        while (!bfsQueue.empty()) {
            int currentVertex = bfsQueue.front();
            bfsQueue.pop();
            cout << static_cast<char>('A' + currentVertex) << " ";

            for (int i = 0; i < numVertices; ++i) {
                if (adjacencyMatrix[currentVertex][i] && !visited[i]) {
                    visited[i] = true;
                    bfsQueue.push(i);
                }
            }
        }
    }
};

int main() {
    ifstream inputFile("data.txt");
    if (!inputFile) {
        cerr << "Failed to open data.txt" << endl;
        return 1;
    }

    int numVertices = 0;
    string str;
    inputFile >> str;

    for (char c : str) {
        if (c != ',') {
            numVertices += 1;
        }
    }

    Graph graph(numVertices);

    // 读取边并构建图
    char src, dest, temp;
    while (inputFile >> src >> temp >> dest) {
        int srcIndex = src - 'A';
        int destIndex = dest - 'A';
        graph.addEdge(srcIndex, destIndex);
    }

    inputFile.close();

    int startVertex = 0; // 以A为起始顶点
    cout << "BFS Traversal: ";
    graph.breadthFirstSearch(startVertex);

    return 0;
}