#include <iostream>
#include <algorithm>
using namespace std;

// 第一种方法，前后缀法
int main()
{
    int n,num[1024];
    cin>>n;
    for(int i=0;i<n;i++)
    {
        cin>>num[i];
    }

    int front[1024],latter[1024];
    front[0]=num[0];
    latter[n-1]=num[n-1];

    for(int i=1;i<n;i++)
    {
        front[i]=max(front[i-1],num[i]);
        latter[n-1-i]=max(latter[n-i],num[n-1-i]);
    }
    //记录前缀和后缀，将理论可装水的体积扣去障碍即可。
    int sum=0;
    for(int i=0;i<n;i++)
    {
        sum+=min(front[i],latter[i])-num[i];
    }

    cout<<sum;
    return 0;

}
