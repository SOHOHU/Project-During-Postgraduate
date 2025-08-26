#include <iostream>
using namespace std;

int main()
{
    int num[10][10],target,m,n;
    cin>>m>>n>>target;
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            cin>>num[i][j];
        }
    }
    int left=0,right=m*n-1,line,row;
    while(left<=right)
    {
        int mid = (left+right)/2-1;
        line = mid/n;
        row = mid % n -1;
        if(num[line][row]<target)
        {
            left=mid+1;
        }else
        {
            right=mid-1;
        }
    }
    line = left/n;
    row = left%n;
    if((left==m*n)||(num[row][line]!=target))
    {
        cout<<-1;
        return 0;
    }
    cout<<left;
return 0;

}