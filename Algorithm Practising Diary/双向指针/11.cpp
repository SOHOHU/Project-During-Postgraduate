#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    //输入每根柱子的高度
    int n,num[1024];
    cin>>n;
    for(int i=0;i<n;i++)
    {
        cin>>num[i];
    }
    int left=0,right=n-1,ans;
    //计算体积
    ans=(right-left) * min(num[left],num[right]);
    while(left<right)
    {
        //将短的一根舍去
        if(num[left]>=num[right])
        {
            right--;
        }else{
            left++;
        }

        int temp=(right-left) * min(num[left],num[right]);
        //更新结果
        ans=max(ans,temp);
    }
    cout<<ans;
    return 0;

}
