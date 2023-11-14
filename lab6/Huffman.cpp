#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

// Huffman树的节点
struct Node {
    char data;  // 字符
    int freq;   // 频率
    Node* left;
    Node* right;

    Node(char data, int freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
};

// 用于比较节点的频率
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

// 生成Huffman树
Node* buildHuffmanTree(const unordered_map<char, int>& freqMap) {
    priority_queue<Node*, vector<Node*>, Compare> pq;

    // 创建叶子节点并将其插入优先队列
    for (const auto& pair : freqMap) {
        Node* leaf = new Node(pair.first, pair.second);
        pq.push(leaf);
    }

    // 构建Huffman树
    while (pq.size() > 1) {
        Node* left = pq.top();
        pq.pop();
        Node* right = pq.top();
        pq.pop();

        Node* parent = new Node('$', left->freq + right->freq);
        parent->left = left;
        parent->right = right;

        pq.push(parent);
    }

    return pq.top();
}

// 生成Huffman编码表
void generateHuffmanCodes(Node* root, string code, unordered_map<char, string>& codes) {
    if (root == nullptr) {
        return;
    }

    if (root->left == nullptr && root->right == nullptr) {
        codes[root->data] = code;
    }

    generateHuffmanCodes(root->left, code + "0", codes);
    generateHuffmanCodes(root->right, code + "1", codes);
}

// 将字符串编码为01序列
string encodeString(const string& str, const unordered_map<char, string>& codes) {
    string encodedStr;
    for (char c : str) {
        encodedStr += codes.at(c);
    }
    return encodedStr;
}

// 计算压缩率
double calculateCompressionRatio(const string& originalStr, const string& encodedStr) {
    int originalBits = originalStr.length() * 8;
    int encodedBits = encodedStr.length();
    return static_cast<double>(encodedBits) / originalBits;
}

// 打印Huffman编码表到文件
void printHuffmanCodesToFile(const unordered_map<char, string>& codes, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& pair : codes) {
            file << pair.first << ": " << pair.second << endl;
        }
        file.close();
    } else {
        cout << "Unable to open the file: " << filename << endl;
    }
}

int main() {
    string originalFilename = "orignal.txt";
    string tableFilename = "table.txt";

    // 读取原始字符串
    ifstream originalFile(originalFilename);
    if (!originalFile.is_open()) {
        cout << "Unable to open the file: " << originalFilename << endl;
        return 1;
    }

    string originalStr((istreambuf_iterator<char>(originalFile)), istreambuf_iterator<char>());
    originalFile.close();

    // 统计字符频率
    unordered_map<char, int> freqMap;
    for (char c : originalStr) {
        freqMap[c]++;
    }

    // 构建Huffman树
    Node* root = buildHuffmanTree(freqMap);

    // 生成Huffman编码表
    unordered_map<char, string> codes;
    generateHuffmanCodes(root, "", codes);

    // 将原始字符串编码为01序列
    string encodedStr = encodeString(originalStr, codes);

    // 打印Huffman编码表到文件
    printHuffmanCodesToFile(codes, tableFilename);

    // 计算压缩率
    double compressionRatio = calculateCompressionRatio(originalStr, encodedStr);

    // 打印压缩率
    cout << "Compression Ratio: " << compressionRatio << endl;

    return 0;
}