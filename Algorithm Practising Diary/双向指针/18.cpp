#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    int n,target,num[1024];
    cin>>n>>target;
    for (int i=0;i<n;i++)
    {
        cin>>num[i];
    }
    sort(num,num+n);
    int left=0,right=n-1,mid1=left+1,mid2=right-1;
    while(1)
    {
        if(num[left]+num[right]>target)
        {
            right--;
            continue;
        }

        mid1=left+1;
        mid2=right-1;
        if(mid1>=mid2)
        {
            break;
        }
        target = target-num[left]-num[right];
        while(mid1<mid2){
            if(num[mid1]+num[mid2]>target)
            {
                mid2--;
            }else if(num[mid1]+num[mid2]<target)
            {
                mid1++;
            }else{
                cout<<"["<<num[left]<<","<<num[mid1]<<","<<num[mid2]<<","<<num[right]<<"]";
                mid2--;
                while (num[mid2]==num[mid2-1])
                    mid2--;
            }
        }
        left++;
    }
    return 0;

}