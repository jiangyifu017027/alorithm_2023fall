#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <queue>

using namespace std;

// 贪心算法生成初始解
vector<int> generateInitialSolution(const vector<int>& tasks, int k, vector<int>& machineTime) {
    int n = tasks.size();
    vector<int> assignment(n, -1);  // 记录任务分配情况

    for (int i = 0; i < n; i++) {
        int minTime = *min_element(machineTime.begin(), machineTime.end());
        int minMachine = distance(machineTime.begin(), min_element(machineTime.begin(), machineTime.end()));
        assignment[i] = minMachine;
        machineTime[minMachine] += tasks[i];
    }

    return assignment;
}

// 回溯法求解最佳调度
void backtrack(const vector<int>& tasks, int n, int k, vector<int>& assignment, vector<int>& bestAssignment, vector<int>& machineTime, int& bestTime, priority_queue<pair<int, vector<int>>>& pq) {
    if (n == tasks.size()) {
        // 所有任务都已分配完毕，更新最佳调度时间
        int maxTime = *max_element(machineTime.begin(), machineTime.end());
        if (maxTime < bestTime) {
            bestTime = maxTime;
            bestAssignment = assignment;
        }
        return;
    }

    for (int i = 0; i < k; i++) {
        // 尝试将任务n分配给第i台机器
        machineTime[i] += tasks[n];
        assignment[n] = i;

        // 剪枝策略1：检查当前机器的完成时间是否已经超过了当前最佳完成时间
        if (machineTime[i] < bestTime) {
            // 剪枝策略2：检查当前机器的完成时间与其他机器的完成时间之差
            int diff = machineTime[i] - *min_element(machineTime.begin(), machineTime.end());
            if (diff <= bestTime) {
                // 继续分配下一个任务
                backtrack(tasks, n + 1, k, assignment, bestAssignment, machineTime, bestTime, pq);
            }
        }

        // 回溯，撤销任务n的分配
        machineTime[i] -= tasks[n];
        assignment[n] = -1;
    }
}

// 求解最佳调度
vector<int> findBestSchedule(const vector<int>& tasks, int k, int& bestTime, const vector<int>& initialSolution) {
    int n = tasks.size();
    vector<int> assignment = initialSolution;  // 记录任务分配情况
    vector<int> bestAssignment(n, -1);  // 记录最佳调度情况
    vector<int> machineTime(k, 0);  // 记录每台机器的完成时间

    // 使用优先队列存储当前最佳完成时间及其对应的任务分配情况
    priority_queue<pair<int, vector<int>>> pq;

    backtrack(tasks, 0, k, assignment, bestAssignment, machineTime, bestTime, pq);

    return bestAssignment;
}

int main() {
    std::vector<std::string> filenames = {"test1.txt", "test2.txt", "test3.txt"};

    for (const std::string& filename : filenames) {
        std::ifstream inputFile(filename);
        if (!inputFile) {
            std::cerr << "Unable to open input file: " << filename << std::endl;
            continue;
        }

        int n, k;
        std::vector<int> tasks;

        // 读取任务数和机器数
        std::string line;
        if (std::getline(inputFile, line)) {
            std::stringstream ss(line);
            ss >> n >> k;
        } else {
            std::cerr << "Invalid input file format: " << filename << std::endl;
            continue;
        }

        // 读取任务时间
        if (std::getline(inputFile, line)) {
            std::stringstream ss(line);
            int taskTime;
            while (ss >> taskTime) {
                tasks.push_back(taskTime);
            }
        } else {
            std::cerr << "Invalid input file format: " << filename << std::endl;
            continue;
        }

        sort(tasks.begin(), tasks.end(), greater<int>());  // 非递增排序

        int bestTime = INT_MAX;  // 记录最佳调度的完成时间
        vector<int> machineTime(k, 0);  // 记录每台机器的完成时间
        vector<int> initialSolution = generateInitialSolution(tasks, k, machineTime);  // 使用贪心算法生成初始解
        bestTime = *max_element(machineTime.begin(), machineTime.end());
        vector<int> bestSchedule = findBestSchedule(tasks, k, bestTime, initialSolution);

        int totalTime = *max_element(bestSchedule.begin(), bestSchedule.end());

        cout << "TotalTime: " << bestTime << endl;
        cout << "Schedule: " << endl;
        for (int i = 0; i < k; i++) {
            cout << "Machine" << i + 1 << ": ";
            for (int j = 0; j < n; j++) {
                if (bestSchedule[j] == i) {
                    cout << "Task" << j + 1 << " ";
                }
            }
            cout << endl;
        }
    }

    return 0;
}