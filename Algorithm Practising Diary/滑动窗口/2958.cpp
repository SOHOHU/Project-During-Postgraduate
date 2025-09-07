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

    //初始化
    for(int right=1;right<n;right++)
    {
        map[num[right]]++;
        //每次加入最右侧的值，必然是最右侧先相应是否重复次数大于k
        if(map[num[right]]>k)
        {
            len--;
        }
        len++;
        ans=max(ans,len);
        //左侧收缩，扣去对应的hash，直到得到符合条件的数组
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


