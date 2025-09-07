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
    // 初始化，先找到最大的元素
    for(int right=1;right<n;right++)
    {
        map[num[right]]++;
        //右扩张，直到找到k次重复最大值
        while(map[max_value]>=k)
        {
            //一旦找到，right右侧全部可以包括进来，left收缩
            ans+=n-right;
            map[num[left]]--;
            left++;
        }

    }
    cout<<ans;
    return 0;

}
