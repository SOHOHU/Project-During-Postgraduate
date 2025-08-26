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
    int left=0,right=n-1,ans;
    ans=(right-left) * min(num[left],num[right]);
    while(left<right)
    {
        if(num[left]>=num[right])
        {
            right--;
        }else{
            left++;
        }

        int temp=(right-left) * min(num[left],num[right]);
        ans=max(ans,temp);
    }
    cout<<ans;
    return 0;
}