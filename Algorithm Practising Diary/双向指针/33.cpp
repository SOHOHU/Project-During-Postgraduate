#include <iostream>
#include <algorithm>
using namespace std;

//前言：循环数组可以使用二分查找，本质上是红蓝染色法，这题用灵神的坐标图秒了
int main()
{
    int num[1024],n,target;
    cin>>n>>target;
    for(int i=0;i<n;i++)
    {
        cin>>num[i];
    }
    int left=0,right=n-1;
    while(left<=right)
    {
        int mid=(left+right)/2;
        //如果mid大于right，那么旋转次数必然大于n/2，且mid两边保持升序
        if(num[mid]>num[right])
        {
            if(target>num[mid])
            {
                //如果target还大于num[mid]，那么target只可能在mid和right之间（准确说是mid和最大值的索引之间，但是此时需要用二分法继续）
                left=mid+1;
            }else if(target<num[mid]&&target>num[right])
            {
                //此时必然在left到mid之间
                right=mid-1;
            }else{
                //其他情况在mid和right之间
                left=mid+1;
            }
        }else{
            // 同理，只需要直到Target所在的区间二分即可
            if(target<num[mid])
            {
                right=mid-1;
            }else if(target>num[mid]&&target<num[right])
            {
                left=mid+1;
            }else{
                right=mid-1;
            }
        }

        if(left==n||num[left]!=target)
        {
            cout<<-1;
        }
    }
    cout<<left;
    return 0;

}
