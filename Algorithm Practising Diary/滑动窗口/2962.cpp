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
    int left=0,ans=0,len=1,max_value=num[left];
    unordered_map<int,int>map;
    map[num[left]]++;
    for(int i=0;i<n;i++)
    {
        max_value=max(max_value,num[i]);
    }

    for(int right=1;right<n;right++)
    {
        map[num[right]]++;
        while(map[max_value]>=k)
        {
            ans+=n-right;
            map[num[left]]--;
            left++;

        }

    }
    cout<<ans;
    return 0;
}