#include <iostream>
#include <algorithm>
using namespace std;

//本题是二分查找一节的内容，虽然可能有多个峰顶，但是题目仅仅要求寻找一个
int main()
{
    int num[1024],n;
    cin>>n;
    for(int i=0;i<n;i++)
    {
        cin>>num[i];
    }
    int left=0,right=n-1;
    //构建数组，定义指针
    //闭区间法
    while(left<=right)
    {
        int mid=(left+right)/2;
        /*定义观察方向为数组索引递增方向，若num[mid]>num[mid]+1，即下坡，则可以认为
        mid之后都是下坡路段*/
        if(num[mid]>=num[mid+1])
        {
            //红蓝染色法，下坡路段（包含可能的峰顶）染蓝色。
            //论域调整，跳过染色区域
            right=mid-1;
        }else{
            //反之染红色，跳过染色区域
            left=mid+1;
        }
    }
    //闭区间二分法left就是答案
    cout<<left;
    return 0;

}
