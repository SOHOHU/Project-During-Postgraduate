#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    int n,num[1024];
    cin>>n;
    for(int i=0;i<n;i++)
    {
        cin>>num[i];
    }
    int left=0,right=n-1,mid=right-1;

    sort(num,num+n);
    while(left<right){
        while(mid>left)
        {
            if(num[left]+num[mid]>num[right])
            {
                int k = left;
                while (mid > k)
                {
                    cout << "[" << num[k] <<","<<num[mid]<<","<<num[right]<<"]"<<endl;
                    k++;
                }
                mid--;
            }else{
                left++;
            }
        }
        right--;
        mid=right-1;
        left=0;
    }
    return 0;
}