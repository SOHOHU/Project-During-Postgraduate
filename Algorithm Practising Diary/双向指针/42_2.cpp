#include <iostream>
#include <algorithm>
using namespace std;
// 双向指针法
int main()
{
    int n,num[1024];
    cin>>n;
    for(int i=0;i<n;i++)
    {
        cin>>num[i];
    }
    
    int front = num[0];
    int latter = num[n-1];
    int left = 0 ,right = n-1,ans=0;
    //当我已知部分前缀和部分后缀的时候，我可以肯定下一个块的前缀必然大于等于已知的最大前缀，后缀同理，因此在双向指针使用的时候可以随时比较最大前和最大后谁大
    //若已知的最大后缀大于最大前缀，则可以从左到右扩展（不断更新前缀直到超过最大后缀），反正则从右到左扩展
    //因此若一个木桶能够确定一个前缀或一个后缀，它的容量就是最大缀和已知取小
    while(left<=right)
    {
        if(front<latter)
        {
            ans+=min(front,latter)-num[left];
            left++;
            front=max(front,num[left]);
        }else{
            ans+=min(front,latter)-num[right];
            right--;
            latter=max(latter,num[right]);
        }
    }
    cout<<ans;
    return 0;

}
