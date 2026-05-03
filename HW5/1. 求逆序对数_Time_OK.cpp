#include <iostream>

using namespace std;

int merge_sort_count(int a[], int temp[], int left, int right);
int merge_and_count(int a[], int temp[], int left, int mid, int right);

void calculate(int group[], int n, int *count)
{
    static int temp[20005];

    *count = 0;

    if (n <= 1)
    {
        return;
    }

    *count = merge_sort_count(group, temp, 0, n - 1);
}

// 数左半段内部逆序数，右半段内部逆序数，跨左右半段逆序数
int merge_sort_count(int a[], int temp[], int left, int right)
{
    if (left >= right)
    {
        return 0;
    }

    int mid = left + (right - left) / 2;
    int inversion = 0; // 逆序数

    inversion += merge_sort_count(a, temp, left, mid);       //  递归计算左半部分的逆序对
    inversion += merge_sort_count(a, temp, mid + 1, right);  //  递归计算右半部分的逆序对
    inversion += merge_and_count(a, temp, left, mid, right); //  合并两部分并计算跨两部分的逆序对

    return inversion;
}

// 合并两个有序段，数跨两部分逆序对，并且把区间变得有序
int merge_and_count(int a[], int temp[], int left, int mid, int right)
{
    int i = left;
    int j = mid + 1;
    int k = left;
    int inversion = 0;

    while (i <= mid && j <= right)
    {
        if (a[i] <= a[j])
        {
            temp[k++] = a[i++];
        }
        else
        {
            temp[k++] = a[j++];

            inversion += mid - i + 1;
        }
    }

    while (i <= mid)
    {
        temp[k++] = a[i++];
    }
    while (j <= right)
    {
        temp[k++] = a[j++];
    }

    for (int p = left; p <= right; p++)
    {
        a[p] = temp[p];
    }

    return inversion;
}
int main()
{
    int n;
    int group[20005];
    int count = 0;
    while (cin >> n)
    {
        for (int i = 0; i < n; i++)
        {
            cin >> group[i];
        }
        if (n == 0)
        {
            exit(0);
        }
        else
        {
            calculate(group, n, &count);
            cout << count << endl;
            count = 0;
        }
    }
    return 0;
}