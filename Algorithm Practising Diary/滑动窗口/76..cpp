#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

bool is_covered(int cnt_s[], int cnt_t[]) {
    for (int i = 'A'; i <= 'Z'; i++) {
        if (cnt_s[i] < cnt_t[i]) {
            return false;
            }
        }
    for (int i = 'a'; i <= 'z'; i++) {
        if (cnt_s[i] < cnt_t[i]) {
            return false;
        }
    }
        return true;
}

int main()
{
    string s,t;
    getline(cin,s);
    getline(cin,t);
    int m = s.length();
    int ans_left = -1, ans_right = m;
    int cnt_s[128]{}; // s 子串字母的出现次数
    int cnt_t[128]{}; // t 中字母的出现次数
    for (char c : t) {
        cnt_t[c]++;
    }
    int left = 0;
    for (int right = 0; right < m; right++) { // 移动子串右端点
        cnt_s[s[right]]++; // 右端点字母移入子串
        while (is_covered(cnt_s, cnt_t)) { // 涵盖
            if (right - left < ans_right - ans_left) { // 找到更短的子串
                ans_left = left; // 记录此时的左右端点
                ans_right = right;
            }
            cnt_s[s[left]]--; // 左端点字母移出子串
            left++;
        }
    }
    cout<<s.substr(ans_left, ans_right - ans_left + 1);
    return 0;
}
        