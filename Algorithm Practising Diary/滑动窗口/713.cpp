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
    for(int right=1;right<n;right++)
    {
        if(k<=1)
        {
            return 0;
        }
        pro*=num[right];
        if(pro>=k&&right!=n-1)
        {
            sum+=right-left;
            while(pro>=k)
            {
                pro/=num[left];
                left++;
            }
        }else if(right==n-1)
        {
            while(pro>=k)
            {
                pro/=num[left];
                left++;
            }

            while(left<=right)
            {
                sum+=right-left+1;
                left++;
            }

        }
    }
    cout<<sum;
    return 0;
    
}