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
    int left=0,right=n-1;
    while(left+1<right)
    {
        int temp=0;
        if(num[left]+num[right]>target)
        {
            right--;
            /* code */
        }else if(num[left]+num[right]==target)
        {
            temp = num[left]+num[left+1]+num[right];
            left++;
        }else{
            int mid=left+1;
            while(mid<right)
            {
                int sum2;
                int sum1=num[left]+num[mid]+num[right]-target;
                if(mid!=right-1)
                {
                    sum2=num[left]+num[mid+1]+num[right]-target;
                    if(sum1*sum2<0)
                    {
                        if(abs(sum1)>abs(sum2)){
                            temp=sum1+target;
                        }else{
                            temp=sum2+target;
                        }
                    }else{
                        temp=sum2+target;
                    }
                }else{
                    temp = sum1+target;
                }
                mid++;
            }
        }
        if(abs(temp-target)<=abs(result-target))
        {
            result=temp;
        }
        left++;
    }
    cout<<result;
    return 0;

}
