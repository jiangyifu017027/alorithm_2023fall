#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

struct Machine {
    int id;
    int time;
    std::vector<int> tasks;
};

bool compareMachine(const Machine& m1, const Machine& m2) {
    return m1.time < m2.time;
}

void scheduleTasks(const std::vector<int>& tasks, int k) {
    int n = tasks.size();
    std::vector<Machine> machines(k);

    // 初始化机器
    for (int i = 0; i < k; ++i) {
        machines[i].id = i + 1;
        machines[i].time = 0;
    }

    // 调度任务
    for (int i = 0; i < n; ++i) {
        // 找到完成时间最早的机器
        auto minMachine = std::min_element(machines.begin(), machines.end(), compareMachine);

        // 将任务分配给该机器
        minMachine->tasks.push_back(i + 1);
        minMachine->time += tasks[i];
    }

    // 输出调度方案和总时间
    int totalTime = 0;
    for (const auto& machine : machines) {
        std::cout << "Machine " << machine.id << " (";
        for (int task : machine.tasks) {
            std::cout << "Task " << task << ",";
        }
        std::cout << ") ";
        totalTime = std::max(totalTime, machine.time);
    }

    std::cout << std::endl;
    std::cout << "Total time: " << totalTime << std::endl;
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

        inputFile.close();

        std::cout << "Processing file: " << filename << std::endl;
        scheduleTasks(tasks, k);
        std::cout << std::endl;
    }

    return 0;
}