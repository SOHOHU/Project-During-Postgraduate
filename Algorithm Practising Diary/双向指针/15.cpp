#include <stdio.h>
#include <stdlib.h>
int compare_int_asc(const void *a, const void *b) {
    int arg1 = *(const int *)a;
    int arg2 = *(const int *)b;
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

int main()
{
    int num[1024],size=0;
    scanf("%d",&size);
    for (int i=0;i<size;i++)
    {
        scanf("%d",&num[i]);
    }
    qsort(num, size, sizeof(int), compare_int_asc);
    int left=0,right=size-1,mid=0;
    while(left<right)
    {
        mid = left + 1;
        int j = left;
        int k = right;
        while (j < k && mid < k)
        {
            if(num[j]+num[k]+num[mid]==0)
            {
                printf("[%d,%d,%d]",num[j],num[mid],num[k]);
                break;
            }else if(num[j]+num[k]+num[mid]<0)
            {
                mid++;
            }else{
                k--;
            }
        }
        left++;
    }
    return 0;
}