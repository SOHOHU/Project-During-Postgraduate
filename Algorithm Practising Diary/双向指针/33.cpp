#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    int num[1024],n,target;
    cin>>n>>target;
    for(int i=0;i<n;i++)
    {
        cin>>num[i];
    }
    int left=0,right=n-1;

    while(left<=right)
    {
        int mid=(left+right)/2;
        if(num[mid]>num[n-1])
        {
            if(target>num[mid])
            {
                left=mid+1;
            }else if(target<num[mid]&&target>num[n-1])
            {
                right=mid-1;
            }else{
                left=mid+1;
            }
        }else{
            if(target<num[mid])
            {
                right=mid-1;
            }else if(target>num[mid]&&target<num[n-1])
            {
                left=mid+1;
            }else{
                right=mid-1;
            }
        }

        if(left==n||num[left]!=target)
        {
            cout<<-1;
        }
    }
    cout<<left;
    return 0;
}