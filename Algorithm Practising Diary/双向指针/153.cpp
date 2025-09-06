#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    //构建数组
    int num[1024],n;
    cin>>n;
    for(int i=0;i<n;i++)
    {
        cin>>num[i];
    }
    int left=0,right=n-1;
    //周期序列也可以使用二分查找
    while(left<=right)
    {
        
        int mid=(left+right)/2;
        //如果中间比最右边大，说明旋转次数大于长度的一半，左边不可能存在最小值
        if(num[mid]>num[n-1])
        {
            left=mid+1;
        }else{
            //反之，右边不可能存在最小值
            right=mid-1;
        }
    }
    cout<<left;
    return 0;

}

