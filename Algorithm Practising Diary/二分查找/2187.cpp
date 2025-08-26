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
    for(int j=1;j<=num[n-1];j++)
    {
        x[j]=j;
    }
    int left=1,right=num[n-1],sum=0,t;
    while(left<=right)
    {
        int mid=(left+right)/2;
        for(int i=0;i<n;i++)
        {
            sum+=x[mid]/num[i];
        }
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