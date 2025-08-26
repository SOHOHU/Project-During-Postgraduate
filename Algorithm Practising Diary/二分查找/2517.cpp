#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

/*
简化答案，任何组合都可以转化为其他两个+p[0]，因为p[0]最小，有它在的最小化最大一定更大
然后就是利用猜答案时输入的猜值看一下能不能组合（利用cnt来看），猜到最小。
*/

class Solution {
public:
    int maximumTastiness(vector<int>& price, int k) {
        auto f = [&](int d) -> int {
            int cnt = 1, pre = price[0]; // 先选一个最小的甜蜜度
            for (int p : price) {
                if (p >= pre + d) { // 可以选
                    cnt++;
                    pre = p; // 上一个选的甜蜜度
                }
            }
            return cnt;
        };

        ranges::sort(price);
        int left = 0;
        int right = (price.back() - price[0]) / (k - 1) + 1;
        while (left + 1 < right) { // 开区间不为空
            // 循环不变量：
            // f(left) >= k
            // f(right) < k
            int mid = left + (right - left) / 2;
            (f(mid) >= k ? left : right) = mid;
        }
        return left; // 最大的满足 f(left) >= k 的数
    }
};
