#include <iostream>
#include <fstream>
#include <queue>

enum Color { RED, BLACK };

struct TNode {
    int key;
    Color color;
    TNode* left;
    TNode* right;
    TNode* p;
};

class RedBlackTree {
private:
    TNode* root;

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
    }

    // 辅助函数：插入修复
    void RB_Insert_Fixup(TNode* z, int& count) {
        std::cout << z->key << ": ";
        while (z->p != nullptr && z->p->color == RED) {
            if (z->p == z->p->p->left) {
                TNode* y = z->p->p->right;
                if (y != nullptr && y->color == RED) {
                    z->p->color = BLACK;  //case1
                    y->color = BLACK;
                    z->p->p->color = RED;
                    z = z->p->p;
                    std::cout << "1 ";
                } else {
                    if (z == z->p->right) {
                        z = z->p;  //case2
                        LeftRotate(z);
                        std::cout << "2 ";
                    }
                    z->p->color = BLACK;  //case3
                    z->p->p->color = RED;
                    RightRotate(z->p->p);
                    std::cout << "3 ";
                    count++;
                }
            } else {
                TNode* y = z->p->p->left;
                if (y != nullptr && y->color == RED) {
                    z->p->color = BLACK;  //case4
                    y->color = BLACK;
                    z->p->p->color = RED;
                    z = z->p->p;
                    std::cout << "4 ";
                } else {
                    if (z == z->p->left) {
                        z = z->p;  //case5
                        RightRotate(z);
                        std::cout << "5 ";
                    }
                    z->p->color = BLACK;
                    z->p->p->color = RED;
                    LeftRotate(z->p->p);  //case6
                    std::cout << "6 ";
                    count++;
                }
            }
        }
        root->color = BLACK;
        std::cout << std::endl;
    }

    // 辅助函数：插入节点
    void RB_Insert(int key, int& count) {
        TNode* z = new TNode;
        z->key = key;
        z->left = nullptr;
        z->right = nullptr;
        z->color = RED;

        TNode* y = nullptr;
        TNode* x = root;
        while (x != nullptr) {
            y = x;
            if (z->key < x->key)
                x = x->left;
            else
                x = x->right;
        }
        z->p = y;
        if (y == nullptr)
            root = z;
        else if (z->key < y->key)
            y->left = z;
        else
            y->right = z;

        RB_Insert_Fixup(z, count);
    }

    // 辅助函数：先序遍历
    void PreorderTraversalHelper(TNode* node, std::ofstream& outfile) {
        if (node == nullptr)
            return;
        outfile << node->key << ", " << (node->color == RED ? "red" : "black") << std::endl;
        PreorderTraversalHelper(node->left, outfile);
        PreorderTraversalHelper(node->right, outfile);
    }

    // 辅助函数：中序遍历
    void InorderTraversalHelper(TNode* node, std::ofstream& outfile) {
        if (node == nullptr)
            return;
        InorderTraversalHelper(node->left, outfile);
        outfile << node->key << ", " << (node->color == RED ? "red" : "black") << std::endl;
        InorderTraversalHelper(node->right, outfile);
    }

    // 辅助函数：层次遍历
    void LevelOrderTraversalHelper(std::ofstream& outfile) {
        if (root == nullptr)
            return;

        std::queue<TNode*> queue;
        queue.push(root);

        while (!queue.empty()) {
            TNode* node = queue.front();
            queue.pop();

            outfile << node->key << ", " << (node->color == RED ? "red" : "black") << std::endl;

            if (node->left != nullptr)
                queue.push(node->left);
            if (node->right != nullptr)
                queue.push(node->right);
        }
    }

public:
    RedBlackTree() {
        root = nullptr;
    }

    // 插入数据并输出结果到文件
    void InsertAndOutput(const std::string& filename1, const std::string& filename2, const std::string& filename3) {
        std::ifstream infile("insert.txt");
        int num;
        infile >> num;

        int count = 0;
        for (int i = 0; i < num; i++) {
            int key;
            infile >> key;
            RB_Insert(key, count);
        }
        infile.close();

        std::ofstream outfile1(filename1);
        std::ofstream outfile2(filename2);
        std::ofstream outfile3(filename3);
        outfile1 << "先序遍历(NLR): " << std::endl;
        PreorderTraversalHelper(root, outfile1);
        outfile2 << "中序遍历(LNR): " << std::endl;
        InorderTraversalHelper(root, outfile2);
        outfile3 << "层次遍历(LOT): " << std::endl;
        LevelOrderTraversalHelper(outfile3);
        outfile1.close();
        outfile2.close();
        outfile3.close();
    }
};

int main() {
    RedBlackTree tree;
    tree.InsertAndOutput("NLR.txt", "LNR.txt", "LOT.txt");

    return 0;
}