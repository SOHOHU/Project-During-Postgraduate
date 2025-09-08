#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    int num[1024],n,x[1024],totaltrip;
    cin>>n>>totaltrip;
    for(int i=0;i<n;i++)
    {
        cin>>num[i];
    }
    sort(num,num+n);
    int max_time=0;
    // 初始化 x为二分猜答案的论域
    for(int j=1;j<=num[n-1];j++)
    {
        x[j]=j;
    }
    // 进行二分查找
    int left=1,right=num[n-1],sum=0,t;
    while(left<=right)
    {
        int mid=(left+right)/2;
        //计算此时的时间
        for(int i=0;i<n;i++)
        {
            sum+=x[mid]/num[i];
        }
        // 红蓝染色法，如果小，则所有x往左的值全部不行，反之右侧全部不行，单调性
        if(sum<totaltrip)
        {
            left=mid+1;
        }else{
            right=mid-1;
        }
        t=sum;
        sum=0;
    }
    for (int i = 0; i < n; i++)
    {
        sum+=x[left]/num[i];
    }
    
    if(sum<totaltrip)
    {
        left++;
    }
    cout << x[left];
    return 0;


}
