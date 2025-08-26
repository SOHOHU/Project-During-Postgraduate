#include <iostream>
#include <string>
#include <cctype>
using namespace std;

string toLowerCase(string& str) {
    string result = str;
    for (char &c : result) {
        c = tolower(c);
    }
    return result;
}

int main()
{
    string a;
    getline(cin,a);
    int left, right=a.length()-1;
    a=toLowerCase(a);
    while(left<=right)
    {
        if(!isalnum(a[left]))
        {
            left++;
            continue;
        }

        if(!isalnum(a[right]))
        {
            right--;
            continue;
        }

        if (a[left]==a[right])
        {
            left++;
            right--;
        }else{
            cout<<"false";
            goto no;
        }
        
    }

    cout<<"true";
    no:
    return 0;
}