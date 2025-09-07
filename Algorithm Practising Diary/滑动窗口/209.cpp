#include <iostream>
using namespace std;

int main()
{
    int n,num[1024],target;
    cin>>n>>target;
    for(int i=0;i<n;i++)
    {
        cin>>num[i];
    }
    int ans=n,left=0,sum=num[left];
    // 初始化
    for(int right=1;right<n;right++)
    {
        sum+=num[right];
        // 达到Target开始左侧收缩，小就右侧扩展，记录最大的len
        while(sum>=target)
        {
            sum-=num[left];
            ans=min(ans,right-left);
            left++;
        }
    }
    for(int i=left-1;i<ans+left;i++)
    {
        cout<<num[i]<<",";
    }
    return 0;

}
