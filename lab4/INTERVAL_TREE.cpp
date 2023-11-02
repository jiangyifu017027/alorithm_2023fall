#include <iostream>
#include <fstream>
#include <queue>
#include <climits>

enum Color { RED, BLACK };

struct Interval {
    int low;
    int high;
};

struct TNode {
    Interval interval;
    int max;
    Color color;
    TNode* left;
    TNode* right;
    TNode* p;
};

class IntervalTree {
private:
    TNode* root;

    // 辅助函数和成员函数
    // 辅助函数：左旋转
    void LeftRotate(TNode* x) {
        TNode* y = x->right;
        x->right = y->left;
        if (y->left != nullptr)
            y->left->p = x;
        y->p = x->p;
        if (x->p == nullptr)
            root = y;
        else if (x == x->p->left)
            x->p->left = y;
        else
            x->p->right = y;
        y->left = x;
        x->p = y;
        x->max = std::max(x->interval.high, std::max(getMax(x->left), getMax(x->right)));
        y->max = std::max(y->interval.high, std::max(getMax(y->left), getMax(y->right)));
    }

    // 辅助函数：右旋转
    void RightRotate(TNode* x) {
        TNode* y = x->left;
        x->left = y->right;
        if (y->right != nullptr)
            y->right->p = x;
        y->p = x->p;
        if (x->p == nullptr)
            root = y;
        else if (x == x->p->left)
            x->p->left = y;
        else
            x->p->right = y;
        y->right = x;
        x->p = y;
        x->max = std::max(x->interval.high, std::max(getMax(x->left), getMax(x->right)));
        y->max = std::max(y->interval.high, std::max(getMax(y->left), getMax(y->right)));
    }

    // 辅助函数：插入修复
    void RB_Insert_Fixup(TNode* z) {
        while (z->p != nullptr && z->p->color == RED) {
            if (z->p == z->p->p->left) {
                TNode* y = z->p->p->right;
                if (y != nullptr && y->color == RED) {
                    z->p->color = BLACK;  // case1
                    y->color = BLACK;
                    z->p->p->color = RED;
                    z = z->p->p;
                } else {
                    if (z == z->p->right) {
                        z = z->p;  // case2
                        LeftRotate(z);
                    }
                    z->p->color = BLACK;  // case3
                    z->p->p->color = RED;
                    RightRotate(z->p->p);
                }
            } else {
                TNode* y = z->p->p->left;
                if (y != nullptr && y->color == RED) {
                    z->p->color = BLACK;  // case4
                    y->color = BLACK;
                    z->p->p->color = RED;
                    z = z->p->p;
                } else {
                    if (z == z->p->left) {
                        z = z->p;  // case5
                        RightRotate(z);
                    }
                    z->p->color = BLACK;
                    z->p->p->color = RED;
                    LeftRotate(z->p->p);  // case6
                }
            }
        }
        root->color = BLACK;
    }

    // 辅助函数：插入节点
    void RB_Insert(TNode* z) {
        TNode* y = nullptr;
        TNode* x = root;
        while (x != nullptr) {
            y = x;
            if (z->interval.low < x->interval.low)
                x = x->left;
            else
                x = x->right;
        }
        z->p = y;
        if (y == nullptr)
            root = z;
        else if (z->interval.low < y->interval.low)
            y->left = z;
        else
            y->right = z;

        RB_Insert_Fixup(z);
    }

    // 辅助函数：获取节点的最大值
    int getMax(TNode* node) {
        if (node == nullptr)
            return INT_MIN;
        return node->max;
    }

    // 辅助函数：判断两个区间是否重叠
    bool isOverlap(Interval& a, Interval& b) {
        return (a.low <= b.high && b.low <= a.high);
    }

    // 辅助函数：查找重叠的区间
    void searchOverlapHelper(TNode* x, Interval& interval, std::vector<Interval>& result) {
        if (x == nullptr)
            return;

        if (isOverlap(x->interval, interval))
            result.push_back(x->interval);

        if (x->left != nullptr)
            searchOverlapHelper(x->left, interval, result);

        if (x->right != nullptr)
            searchOverlapHelper(x->right, interval, result);
    }

public:
    // 构造函数和成员函数
    // 构造函数
    IntervalTree() {
        root = nullptr;
    }

    // 插入区间
    void insert(Interval interval) {
        TNode* z = new TNode;
        z->interval = interval;
        z->max = interval.high;
        z->color = RED;
        z->left = nullptr;
        z->right = nullptr;
        z->p = nullptr;

        RB_Insert(z);
    }

    // 查找重叠的区间
    std::vector<Interval> searchOverlap(Interval interval) {
        std::vector<Interval> result;
        searchOverlapHelper(root, interval, result);
        return result;
    }

    // 从文件中读取插入的区间
    void readInsertFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return;
        }

        int count;
        file >> count;
        for (int i = 0; i < count; ++i) {
            Interval interval;
            file >> interval.low >> interval.high;
            insert(interval);
        }

        file.close();
    }

};

int main() {
    IntervalTree tree;

    // 从文件中读取插入的区间
    tree.readInsertFromFile("insert.txt");

    // 从控制台读取待查询区间
    Interval query;
    std::cout << "Enter query interval (low high): ";
    std::cin >> query.low >> query.high;

    // 查找重叠的区间
    std::vector<Interval> overlappedIntervals = tree.searchOverlap(query);

    std::cout << "Overlapped intervals:" << std::endl;
    for (const auto& interval : overlappedIntervals) {
        std::cout << "[" << interval.low << ", " << interval.high << "]" << std::endl;
    }

    return 0;
}