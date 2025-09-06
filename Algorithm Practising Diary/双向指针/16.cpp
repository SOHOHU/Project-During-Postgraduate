#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    int n,target,num[1024],result=0;
    cin>>n>>target;
    for(int i=0;i<n;i++)
    {
        cin>>num[i];
    }
    sort(num,num+n);
    int left=0,right=n-1,mid=left+1;
    while(left<=right)
    {   
        mid=left+1;
        right=n-1;
        if(num[left]+num[mid]+num[right]==target)
        {
            //直接找到答案
            cout<<0<<"["<<left<<","<<mid<<","<<right<<"]"<<end;
            break;
        }else｛
            //三数求和
            int dis=num[left]+num[mid]+num[right]-target;
            while(mid<=right)
            {
                //记得时刻更新最小距离
                if(num[left]+num[mid]+num[right]<0)
                {
                    mid++;
                    dis=min(dis,num[left]+num[mid]+num[right]-target);
                }else if(num[left]+num[mid]+num[right]>0)
                {
                    right--;
                    dis=min(dis,num[left]+num[mid]+num[right]-target);
                }else{
                    cout<<0<<"["<<left<<","<<mid<<","<<right<<"]"<<end;
                    return 0;
                }
            }
        ｝
        left++;
    }
    cout<<dis<<"["<<left<<","<<mid<<","<<right<<"]"<<end;
    return 0;
}

