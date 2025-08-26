#include <iostream>
#include <unordered_map>
using namespace std;

int main()
{
    int n,num[1024],k;
    cin>>n>>k;
    for(int i=0;i<n;i++)
    {
        cin>>num[i];
    }
    int left=0,ans=0,len=1;
    unordered_map<int,int>map;
    map[num[left]]++;
    for(int right=1;right<n;right++)
    {
        map[num[right]]++;
        len++;
        if(map[0]<=k)
        {
            ans=max(ans,len);
        }

        while(map[0]>k)
        {
            map[num[left]]--;
            left++;
            len--;
        }
    }
    cout<<ans;
    return 0;
}