#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// 时间复杂度O(mn)，空间复杂度O(mn)
int lcs1(const string& text1, const string& text2) {
    int m = text1.length();
    int n = text2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (text1[i - 1] == text2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    return dp[m][n];
}

// 时间复杂度O(mn)，空间复杂度O(2*min(m,n))
int lcs2(const string& text1, const string& text2) {
    int m = text1.length();
    int n = text2.length();
    vector<vector<int>> dp(2, vector<int>(n + 1, 0));

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (text1[i - 1] == text2[j - 1]) {
                dp[i % 2][j] = dp[(i - 1) % 2][j - 1] + 1;
            } else {
                dp[i % 2][j] = max(dp[(i - 1) % 2][j], dp[i % 2][j - 1]);
            }
        }
    }

    return dp[m % 2][n];
}

// 时间复杂度O(mn)，空间复杂度O(min(m,n))
int lcs3(const string& text1, const string& text2) {
    int m = text1.length();
    int n = text2.length();
    if (m < n) {
        return lcs3(text2, text1);
    }
    vector<int> dp(n + 1, 0);

    for (int i = 1; i <= m; i++) {
        int pre = 0;
        for (int j = 1; j <= n; j++) {
            int temp = dp[j];
            if (text1[i - 1] == text2[j - 1]) {
                dp[j] = pre + 1;
            } else {
                dp[j] = max(dp[j], dp[j - 1]);
            }
            pre = temp;
        }
    }

    return dp[n];
}

int main() {
    ifstream inputFile("data.txt");
    if (!inputFile) {
        cout << "Failed to open the file." << endl;
        return 0;
    }

    string text1, text2;
    getline(inputFile, text1);
    getline(inputFile, text2);

    inputFile.close();

    int length1 = lcs1(text1, text2);
    cout << "LCS length (O(mn), O(mn)): " << length1 << endl;

    int length2 = lcs2(text1, text2);
    cout << "LCS length (O(mn), O(2*min(m,n))): " << length2 << endl;

    int length3 = lcs3(text1, text2);
    cout << "LCS length (O(mn), O(min(m,n))): " << length3 << endl;

    // 输出最长公共子序列
    vector<char> lcs;
    int i = text1.length(), j = text2.length();
    while (i > 0 && j > 0) {
        if (text1[i - 1] == text2[j - 1]) {
            lcs.push_back(text1[i - 1]);
            i--;
            j--;
        } else if (lcs1(text1.substr(0, i), text2.substr(0, j)) == lcs1(text1.substr(0, i - 1), text2.substr(0, j))) {
            i--;
        } else {
            j--;
        }
    }
    reverse(lcs.begin(), lcs.end());

    for (auto c : lcs) {
        cout << c;
    }

    return 0;
}