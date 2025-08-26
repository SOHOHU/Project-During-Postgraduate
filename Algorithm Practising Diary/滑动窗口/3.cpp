#include <iostream>
#include <unordered_map>
using namespace std;

int main()
{
    string str;
    getline(cin,str);
    unordered_map<char,int>map;
    int left=0,ans=0,len=1;
    map[str[left]]=1;
    for(int right=1;right<str.length();right++)
    {
        map[str[right]]++;
        if(map[str[right]]>1)
        {
            len--;
        }
        len++;
        ans=max(len,ans);
        while(map[str[right]]>1)
        {
            left++;
            len--;
            map[str[left]]--;
        }
    }

    cout<<ans;
    return 0;

}