#include <iostream>
#include <algorithm>
using namespace std;

int sum(int pile[],int n,int k)
{
    int total=0;
    for(int i=0;i<n;i++)
    {
        if(pile[i]%k==0)
        {
            total+=pile[i]/k;
        }else{
            total+=pile[i]/k+1;
        }
    }
    return total;
}

int main()
{
    int n,num[1024],h=0;
    cin>>n>>h;
    for(int i=0;i<n;i++)
    {
        cin>>num[i];
    }
    sort(num,num+n);
    int left=0,right=num[n-1];
    while(left<=right)
    {
        int mid=(left+right)/2;
        int total=sum(num,n,mid);
        if(total<=h)
        {
            right=mid-1;
        }else{
            left=mid+1;
        }
    }
    cout<<left;
    return 0;
}
    
