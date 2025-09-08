#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    int n,m,spell[1024],position[1024],success=0;
    cin>>n>>m>>success;
    for(int i=0;i<n;i++)
    {
        cin>>spell[i];
    }
    for(int i=0;i<m;i++)
    {
        cin>>position[i];
    }
    int left=0,right=m-1,ans=0,len=0;
    sort(position,position+m);
    //初始化，因为spell要输出顺序，所以只能对position排序
    //使用红蓝染色法，找到最小的position可以，蓝色区域（大于）都可以
    for(int i=0;i<n;i++)
    {
        int eff=0;
        if(success%spell[i]==0)
        {
            eff=success/spell[i];
        }else{
            eff=success/spell[i]+1;
        }
        //对eff向上取整数，这个是至少需要达到的position
        while(left<=right)
        {
            //二分查找这个eff
            int mid=(left+right)/2;
            if(position[mid]>=eff)
            {
                len+=right-mid+1;
                right=mid-1;
            }else{
                left=mid+1;
            }
        }
        cout<<len<<",";
        ans+=len;
        right=m-1;
        left=0;
        len=0;

    }
    return 0;

}
