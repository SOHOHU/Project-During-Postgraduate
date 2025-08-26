#include <iostream>
using namespace std;

int main()
{
    int n,num[1024],ans=0,h=0;
    cin>>n;
    for(int i=0;i<n;i++)
    {
        cin>>num[i];
    }
    int left=0,right=n-1;
    for(int j=1;j<=n;j++)
    {
        while(left<=right)
        {
            int mid=(left+right)/2;
            if(num[mid]<j)
            {
                left=mid+1;
            }else{
                right=mid-1;
            }
        }
        if(n-left>=j)
        {
            h=j;
        }else{
            break;
        }
        ans=max(ans,h);
        left=0;
        right=n-1;
    }
    cout<<ans;
    return 0;
}