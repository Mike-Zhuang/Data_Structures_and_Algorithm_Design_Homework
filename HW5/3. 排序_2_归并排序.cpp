/**
 * @file    template.cpp
 * @name    p57模板程序
 * @date    2022-11-22
 */

#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <set>
using namespace std;

/********************************/
/*     以下是你需要提交的代码     */
/********************************/
class Solution
{
private:
    void merge_range(vector<int> &a, vector<int> &temp, int left, int mid, int right)
    {
        int i = left;
        int j = mid + 1;
        int k = left;

        while (i <= mid && j <= right)
        {
            if (a[i] <= a[j])
            {
                temp[k] = a[i];
                i++;
                k++;
            }
            else
            {
                temp[k] = a[j];
                j++;
                k++;
            }
        }

        while (i <= mid)
        {
            temp[k] = a[i];
            i++;
            k++;
        }

        while (j <= right)
        {
            temp[k] = a[j];
            j++;
            k++;
        }
        for (int p = left; p <= right; p++)
        {
            a[p] = temp[p];
        }
    }

    void merge_sort(vector<int> &a, vector<int> &temp, int left, int right)
    {
        if (left >= right)
        {
            return;
        }

        int mid = left + (right - left) / 2;

        merge_sort(a, temp, left, mid);
        merge_sort(a, temp, mid + 1, right);
        merge_range(a, temp, left, mid, right);
    }

public:
    std::vector<int> mySort(std::vector<int> &nums)
    {
        // 这里填写你的代码

        // 2. 归并排序
        vector<int> sorted = nums;
        vector<int> temp(sorted.size());
        if (!sorted.empty())
        {
            merge_sort(sorted, temp, 0, (int)sorted.size() - 1);
        }
        return sorted;
    }
};
/********************************/
/*     以上是你需要提交的代码     */
/********************************/

int main()
{
    int n;
    std::cin >> n;
    std::vector<int> nums(n);
    for (int i = 0; i < n; i++)
    {
        std::cin >> nums[i];
    }
    Solution s;
    std::vector<int> after_sort(s.mySort(nums));
    std::cout << after_sort[0];
    for (int i = 1; i < n; i++)
    {
        std::cout << ' ' << after_sort[i];
    }
    std::cout << std::endl;
    return 0;
}