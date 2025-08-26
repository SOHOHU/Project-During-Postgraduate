#include <iostream>
#include <unordered_map>
using namespace std;

int main()
{
    int num[1024],k,n;
    cin>>n>>k;
    for(int i=0;i<n;i++)
    {
        cin>>num[i];
    }
    int left=0;
    unordered_map<int,int>map;
    map[num[left]]++;
    int ans=0,len=1;
    for(int right=1;right<n;right++)
    {
        map[num[right]]++;
        if(map[num[right]]>k)
        {
            len--;
        }
        len++;
        ans=max(ans,len);
        while(map[num[right]]>k)
        {
            map[num[left]]--;
            left++;
            len--;
        }
    }

    for(int i=left;i<ans+left;i++)
    {
        cout<<num[i]<<",";
    }
    return 0;
}

