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
    int front = num[0];
    int latter = num[n-1];
    int left = 0 ,right = n-1,ans=0;
    while(left<=right)
    {
        if(front<latter)
        {
            ans+=min(front,latter)-num[left];
            left++;
            front=max(front,num[left]);
        }else{
            ans+=min(front,latter)-num[right];
            right--;
            latter=max(latter,num[right]);
        }
    }
    cout<<ans;
    return 0;
}