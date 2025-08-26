#include <iostream>
#include <algorithm>
using namespace std;

/*双向不等式使用同前缀法*/


int main()
{
    int n,num[1024],lower=0,upper=0;
    cin>>n>>lower>>upper;
    for(int i=0;i<n;i++)
    {
        cin>>num[i];
    }
    int ans=0,left=0,right=n-1,l,r;
    sort(num,num+n);
    for(int j=0;j<n;j++)
    {
        left=j;
        right=n-1;
        while(left<=right)
        {
            int mid=(left+right)/2;
            if(num[mid]<lower-num[j])
            {
                left=mid+1;
            }else{
                right=mid-1;
            }
        }
        l=left-1-j;
        left=j;
        right=n-1;
        while(left<=right)
        {
            int mid=(left+right)/2;
            if(num[mid]<upper-num[j])
            {
                left=mid+1;
            }else{
                right=mid-1;
            }
        }
        if(num[left]==upper-num[j])
        {
            r=left-j;
        }else{
            r=left-1-j;
        }

        if(l<0||r<0)
        {
            continue;
        }
        ans+=r-l;
    }
    cout<<ans;
    return 0;
}
