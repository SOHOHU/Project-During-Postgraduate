#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

int main()
{
    string str;
    getline(cin,str);
    int left=0,temp=0,ans=0,len=1;
    for(int right=1;right<str.length();right++)
    {
        len++;
        if(str[right]==str[right-1])
        {
            temp++;
        }
        if(temp>1)
        {
            len--;
        }
        ans = max(ans,len);

        while(temp>1)
        {
            if(str[left]==str[left+1])
            {
                temp--;
            }
            left++;
            len--;
        }
    }
    cout<<ans;
    return 0;
}