#include <iostream>
using namespace std;

int main()
{
    int n,num[1024],neg=0,pos=0,ans;
    cin>>n;
    for(int i=0;i<n;i++)
    {
        cin>>num[i];
    }
    int left=0,right=n-1;
    while(left<=right)
    {
        int mid=(left+right)/2;
        if(num[mid]<0)
        {
            neg+=mid-left+1;
            left=mid+1;
        }else{
            pos+=right-mid+1;
            right=mid-1;
        }
    }
    ans=max(neg,pos);
    cout<<ans;
    return 0;
}
