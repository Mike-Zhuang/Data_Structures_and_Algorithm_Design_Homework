#include <iostream>
using namespace std;

// 从小到大排序
void sort(int n, int nums[])
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (nums[j] > nums[j + 1])
            {
                int temp = nums[j];
                nums[j] = nums[j + 1];
                nums[j + 1] = temp;
            }
        }
    }
}

void frontsum(int n, int nums[], int nums_sum[])
{
    int temp_sum = 0;
    for (int i = 0; i < n; i++)
    {
        temp_sum += nums[i];
        nums_sum[i] = temp_sum;
    }
}

int main()
{
    int n, m;
    cin >> n >> m;
    int nums[n], queries[m], nums_sum[n], answer[m];
    for (int i = 0; i < n; i++)
    {
        cin >> nums[i];
    }
    for (int i = 0; i < m; i++)
    {
        cin >> queries[i];
    }

    sort(n, nums);
    frontsum(n, nums, nums_sum);

    /* for (int i = 0; i < m; i++)
    {
        answer[i] = 0;
        for (int j = 0; j < n; j++)
        {
            if (nums_sum[j] <= queries[i])
            {
                answer[i] = j + 1;
            }
        }
    } */

    // 从普通遍历改成二分查找
    for (int i = 0; i < m; i++)
    {
        int left = 0, right = n - 1;
        answer[i] = 0;
        while (left <= right)
        {
            int mid = (left + right) / 2;
            if (nums_sum[mid] <= queries[i])
            {
                answer[i] = mid + 1;
                left = mid + 1;
            }
            else
            {
                right = mid - 1;
            }
        }
    }

    for (int i = 0; i < m; i++)
    {
        cout << answer[i] << " ";
    }
    return 0;
}