#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

/*
搭积木法，把右边的一个柱积木一个个往左边搬
堆到一旦有柱子超过limit（二分猜法的猜答案）就不能实现
*/
class Solution {
private:
    int size; //长度
public:
    int minimizeArrayValue(vector<int> &nums) {
        auto check = [&](int limit) -> bool {
            long long extra = 0;
            for (int i = nums.size() - 1; i > 0; i--) {
                extra = max(nums[i] + extra - limit, 0LL);
            }
            return nums[0] + extra <= limit;
        };
        // 开区间二分，原理见 https://www.bilibili.com/video/BV1AP41137w7/
        int left = -1, right = size;
        while (left + 1 < right) {
            int mid = (left + right) / 2;
            (check(mid) ? right : left) = mid;
        }
        return right;
    }
};
