#include <iostream>
using namespace std;

int main()
{
    //定义数组
    int n,num[1024];
    cin>>n;
    for(int i=0;i<n;i++)
    {
        cin>>num[i];
    }
    int a,b;
    int sum_a=0,sum_b=0;
    cin>>a>>b;
    int left=0,right=n-1,a_before=a,b_before=b;
    //双向指针开始
    while(left<right)
    {
        //A的水不够
        if(a_before<num[left])
        {
            a_before=a;
            sum_a++;
            continue;
        }
        //B的水不够
        if(b_before<num[right])
        {
            b_before=b;
            sum_b++;
            continue;
        }
        //A和B够就浇水
        a_before-=num[left];
        b_before-=num[right];
        left++;
        right--;
    }
    //到达同一植物
    if(left==right)
    {
        //水多先浇
        if(a_before>b_before)
        {
            alice:
            if(a_before<num[left])
            {
                a_before=a;
                sum_a++;
            }
            a_before-=num[left];
        }else if(a_before<b_before)
        {
            if(b_before<num[right])
            {
                b_before=b;
                sum_b++;
            }
            b_before-=num[right];
        }else{
            goto alice;
        }
    }
    
    cout<<sum_a<<","<<sum_b;

    return 0;


}
