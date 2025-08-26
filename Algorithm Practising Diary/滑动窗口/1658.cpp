#include<iostream>
using namespace std;

int total(int arr[],int begin,int end)
{
    int sum=0;
    for(int i=begin;i<end;i++)
    {
        sum+=arr[i];
    }
    return sum;
}

int main()
{
    int n,num[1024],x;
    cin>>n>>x;
    for(int i=0;i<n;i++)
    {
        cin>>num[i];
    }
    /*逆向思维，该方法可以扩展到所有两头操作的题目*/
    int left=0,sum1=total(num,0,n)-x,sum2=0,len=1,ans=-1;
    for(int right=1;right<n;right++)
    {
        sum2=total(num,left,right+1);
        len++;
        while(sum2>sum1)
        {
            sum2-=num[left];
            left++;
            len--;
        }
        /*没到等就让len继续下去，不会改变ans*/
        if(sum2==sum1)
        {
            ans=max(ans,len);
        }
    }
    if(ans<0)
    {
        cout<<-1;
        
    }else{
        cout<<n-ans;
    }
    
    return 0;
}