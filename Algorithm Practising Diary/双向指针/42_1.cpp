#include <iostream>
#include <algorithm>
using namespace std;

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
    int sum=0;
    for(int i=0;i<n;i++)
    {
        sum+=min(front[i],latter[i])-num[i];
    }

    cout<<sum;
    return 0;
}