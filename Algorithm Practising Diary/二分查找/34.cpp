#include <iostream>
using namespace std;

int main()
{
    int n,num[1024],x;
    cin>>n>>x;
    for(int i=0;i<n;i++)
    {
        cin>>num[i];
    }
    int left=0,right=n-1,mid=(left+right)/2;
    while(left<=right)
    {
        mid=(left+right)/2;
        if(num[mid]<x)
        {
            left=mid+1;
        }else{
            right=mid-1;
        }
    }
    if(n%2==0)
    {
        right=2*mid-left+1;
    }else{
        right=2*mid-left;
    }

    if(num[left]!=x)
    {
        cout<<-1;
    }else{
        cout<<"["<<left<<","<<right<<"]";
    }

    return 0;

}