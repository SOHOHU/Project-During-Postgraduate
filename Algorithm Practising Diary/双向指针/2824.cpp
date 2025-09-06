#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    int n,target,num[1024];
    cin>>n>>target;
    for(int i=0;i<n;i++)
    {
        cin>>num[i];
    }
    int left=0,right=n-1;
    sort(num,num+n);
    // 记得双指针之前要先排序
    while(left<right)
    { 
        //将等于或者大于Target的舍去即可，小于可以遍历所有情况记录
        int k = left+1;
        if (num[left]+num[right]<target)
        {
            while(k<=right)
            {
                cout<<"["<<num[left]<<","<<num[k]<<"]";
                k++;
            }
            left++;
        }else{
            right--;
        }
    }

    return 0;


}
