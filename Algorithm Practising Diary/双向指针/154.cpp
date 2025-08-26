/*
和153一模一样，只需要注意num[mid]==num[n-1]这一种情况即可（中间相邻数值相等二分法是可以正确找到目标的，不影响）
只需要当这两个相等的时候right--即可（因为只有第一次算mid才有这情况发生，right--就是直接跳过num[n-1]）
*/

#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    int num[1024],n;
    cin>>n;
    for(int i=0;i<n;i++)
    {
        cin>>num[i];
    }
    int left=0,right=n-1;
    while(left<=right)
    {
        int mid=(left+right)/2;
        if(num[mid]==num[n-1])
        {
            right--;
        }

        if(num[mid]>num[n-1])
        {
            left=mid+1;
        }else{
            right=mid-1;
        }
    }
    cout<<left;
    return 0;
}