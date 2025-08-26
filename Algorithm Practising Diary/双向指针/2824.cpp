#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    int n,target,num[1024];
    cin>>n>>target;
    for(int i=0;i<n;i++)
    {
        cin>>num[i];
    }
    int left=0,right=n-1;
    sort(num,num+n);
    while(left<right)
    { 

        int k = left+1;
        if (num[left]+num[right]<target)
        {
            while(k<=right)
            {
                cout<<"["<<num[left]<<","<<num[k]<<"]";
                k++;
            }
            left++;
        }else{
            right--;
        }
    }

    return 0;

}