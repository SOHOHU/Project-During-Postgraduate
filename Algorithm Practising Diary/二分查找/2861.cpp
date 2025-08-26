#include <iostream>
#include <algorithm>
using namespace std;

bool check(int machine[],int n,int ini[],int cost[],int budget,int num)
{
    int total_cost=0,sum=0;
    while(total_cost<=budget)
    {
        for(int i=0;i<n;i++)
        {
            if(machine[i]>ini[i])
            {
                total_cost+=(machine[i]-ini[i])*cost[i];
                ini[i]=machine[i];
            }
        }
        for(int i=0;i<n;i++)
        {
            ini[i]=machine[i]-ini[i];
        }
        sum++;
    }
    if(sum>=num)
    {
        return true;
    }else{
        return false;
    }
    
}

int main()
{
    int n,k,compose[1024][1024],ini[1024],cost[1024],budget;
    cin>>n>>k>>budget;
    for(int i=0;i<k;i++)
    {
        for(int j=0;j<n;j++)
        {
            cin>>compose[i][j];
        }
    }
    for(int i=0;i<n;i++)
    {
        cin>>ini[i];
    }
    for(int i=0;i<n;i++)
    {
        cin>>cost[i];
    }
    int left=0,right=budget+k;
    int sum=0,ans=0;
    for(int i=0;i<k;i++)
    {
        while(left<=right)
        {
            int mid=(left+right)/2;
            if(check(compose[i],n,ini,cost,budget,mid)){  
                left=mid+1;
            }else{
                right=mid-1;
            }
        }
        ans=max(left,ans);
    }
cout<<ans;
return 0;
}