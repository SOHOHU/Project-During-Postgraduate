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
    while(left<right)
    {
        //控制R不变
        while(left<mid2)
        {
            //控制L不变
            if(num[left]+num[right]+num[mid1]+num[mid2]==target)
            {
                //直接成立；
                cout<<left<<" "<<right<<" "<<mid1<<" "<<mid2<<"ok"<<endl;
                break；
            }else{
                //两个变量进行双向指针查找
                int temp=target+num[left]+num[right];
                while(mid1<mid2)
                {
                    if(num[mid1]+num[mid2]>temp)
                    {
                        mid2--;
                    }else if(num[mid1]+num[mid2]<temp){
                        mid1++
                    }else{
                        cout<<left<<" "<<right<<" "<<mid1<<" "<<mid2<<"ok"<<endl;
                        break；
                    }
                }
            }
            //找不到跟新参数
            left++;
            mid1=left+1;
            mid2=right-1;
        }
        //找不到跟新参数
        right--;
        left=0;
        mid2=right-1;
        mid1=left+1；
    }
    return 0;


}
