#include<iostream>
#include<unordered_map>
using namespace std;


int main()
{
    int n,num[1024],k;
    cin>>n>>k;
    for(int i=0;i<n;i++)
    {
        cin>>num[i];
    }
    int left=0,ans=0,score=num[left],sum=0;
    if(num[left]<k){
        ans++;
    }

    for(int right=1;right<n;right++)
    {
        for(int i=left;i<=right;i++)
        {
            sum+=num[i];
        }
        score=sum*(right-left+1);
        while(score>=k)
        {
            left++;
            sum=0;
            for(int i=left;i<=right;i++)
            {
                sum+=num[i];
            }
            score=sum*(right-left+1);
        }
        int len=right-left+1;
        ans+=len;
        sum=0;
    }
    cout<<ans;
    return 0;
}