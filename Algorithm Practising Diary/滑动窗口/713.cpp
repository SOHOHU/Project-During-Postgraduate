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
