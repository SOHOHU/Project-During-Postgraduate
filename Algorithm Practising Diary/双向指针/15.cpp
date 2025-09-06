#include <stdio.h>
#include <stdlib.h>

// Cqsort语言排序函数
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
    //排序
    int left=0,right=size-1,mid=0;
    while(left<right)
    {
        //L+M+R=0即M+R=-L,我们使用控制变量法，固定L，用双向指针找到这个满足式子的LMR
        mid = left + 1;
        int j = left;
        int k = right;
        while (j < k && mid < k)
        {
            if(num[j]+num[k]+num[mid]==0)
            {
                printf("[%d,%d,%d]",num[j],num[mid],num[k]);
                break;
            //如果M+R<-L,则此时所有小于R的数都被淘汰，根据小舍左大舍右的二数求和经验，指针右移
            }else if(num[j]+num[k]+num[mid]<0)
            {
                mid++;
            }else{
                k--;
            }
        }
        //M和R遍历结束后重新遍历
        left++;
    }
    return 0;

}
