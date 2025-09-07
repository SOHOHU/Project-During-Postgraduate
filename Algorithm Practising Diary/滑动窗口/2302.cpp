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
    //初始化，开始判断第一个（因为right是开区间）
    for(int right=1;right<n;right++)
    {
        for(int i=left;i<=right;i++)
        {
            sum+=num[i];
        }
        score=sum*(right-left+1);
        //超过重新算分，左侧收缩
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
        //这里审题错了，让求的是数目不是最长序列长，不过原理是一样的
        ans+=len;
        sum=0;
    }
    cout<<ans;
    return 0;

}
