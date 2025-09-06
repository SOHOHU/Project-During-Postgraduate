/*
用二分法实现
left和right确定行二分的mid
对mid1找最大值（如果是非最高峰，查找的时候肯定会绕一圈回到mid找最高峰，死循环了）
因为是求最大所以用遍历了，不用列二分了
然后对这个最高峰使用162法和上下锋对比，这样必找到锋

比较简单，不写了

*/

#include <iostream>
#include <stdlib>
#include <algorithm>
using namespace std;
int main()
{
  int arr[10][10],n;
  for(int i=0;i<n;i++)
  {
      arr[0][i]=-1;
      arr[i][0]=-1;
      arr[n-1][i]=-1;
      arr[i][n-1]=-1;
  }
  for(int i=1;i<n;i++)
  {
      for(int j=1;j<n;j++)
      {
          cin>>arr[i][j];
      }
  }
  //输入数组
  int left=0,right=n-1;
  for(int i=0;i<n;i++)
  {
      while(left<right)
      {
          int mid=(left+right)/2;
          if(arr[i][mid]<arr[i][mid])
          {
            
          }
      }
  }
  
}
