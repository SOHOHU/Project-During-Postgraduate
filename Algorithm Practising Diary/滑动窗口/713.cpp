#include <iostream>
using namespace std;

int main()
{
    int n,num[1024],k;
    cin>>n>>k;
    for(int i=0;i<n;i++)
    {
        cin>>num[i];
    }
    int left=0,sum=0;
    int pro=num[left];
    //初始化
    //为什么一定要用开区间？因为使用开区间在right到底的时候依然可以讨论好所有的答案
    for(int right=1;right<n;right++)
    {
        //右侧扩张
        if(k<=1)
        {
            return 0;
        }
        pro*=num[right];
        if(pro>=k)
        {
            //此时序列内所有的子序列都是成立的
            sum+=right-left;
            //左收缩
            while(pro>=k)
            {
                pro/=num[left];
                left++;
            }
        }
    }
    cout<<sum;
    return 0;
    

}

