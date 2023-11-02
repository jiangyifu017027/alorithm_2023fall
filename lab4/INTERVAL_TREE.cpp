#include <iostream>
#include <set>
#include <vector>
#include <fstream>

// 区间节点的结构体定义
struct IntervalNode {
    int low;      // 区间的下界
    int high;     // 区间的上界
    int max_high; // 当前节点及其子树中区间的最大上界
    IntervalNode* left;
    IntervalNode* right;
};

// 区间树类
class IntervalTree {
public:
    IntervalTree() : root(nullptr) {}

    // 插入一个区间
    void insert(int low, int high) {
        root = insertHelper(root, low, high);
    }

    // 查找与给定区间重叠的区间
    std::vector<IntervalNode*> findOverlappingIntervals(int low, int high) {
        std::vector<IntervalNode*> result;
        findOverlappingIntervalsHelper(root, low, high, result);
        return result;
    }

private:
    // 插入辅助函数
    IntervalNode* insertHelper(IntervalNode* node, int low, int high) {
        if (node == nullptr) {
            node = new IntervalNode();
            node->low = low;
            node->high = high;
            node->max_high = high;
            node->left = nullptr;
            node->right = nullptr;
        } else if (low < node->low) {
            node->left = insertHelper(node->left, low, high);
            if (node->left->max_high > node->max_high) {
                node->max_high = node->left->max_high;
            }
        } else {
            node->right = insertHelper(node->right, low, high);
            if (node->right->max_high > node->max_high) {
                node->max_high = node->right->max_high;
            }
        }
        return node;
    }

    // 查找重叠区间的辅助函数
    void findOverlappingIntervalsHelper(IntervalNode* node, int low, int high, std::vector<IntervalNode*>& result) {
        if (node == nullptr) {
            return;
        }
        if (node->low <= high && node->high >= low) {
            result.push_back(node);
        }
        if (node->left != nullptr && node->left->max_high >= low) {
            findOverlappingIntervalsHelper(node->left, low, high, result);
        }
        if (node->right != nullptr && node->right->max_high >= low) {
            findOverlappingIntervalsHelper(node->right, low, high, result);
        }
    }

    IntervalNode* root;
};

int main() {
    IntervalTree intervalTree;

    // 从文件读取待插入数据
    std::ifstream file("insert.txt");
    int numIntervals;
    file >> numIntervals;
    for (int i = 0; i < numIntervals; i++) {
        int low, high;
        file >> low >> high;
        intervalTree.insert(low, high);
    }
    file.close();

    // 接受用户输入并进行查询
    while (true) {
        int queryLow, queryHigh;
        std::cout << "Please enter the lower bound and upper bound of the interval you want to query (separated by a space):";
        std::cin >> queryLow >> queryHigh;

        std::vector<IntervalNode*> result = intervalTree.findOverlappingIntervals(queryLow, queryHigh);

        if (result.empty()) {
            std::cout << "nil" << std::endl;
        } else {
            for (const auto& node : result) {
                std::cout << "[" << node->low << ", " << node->high << "] ";
            }
            std::cout << std::endl;
        }
    }

    return 0;
}