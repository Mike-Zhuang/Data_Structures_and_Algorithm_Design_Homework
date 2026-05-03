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
    int partition(vector<int> &a, int left, int right)
    {
        int pivot = a[left + (right - left) / 2];

        int i = left - 1;
        int j = right + 1;

        while (true)
        {
            do
            {
                i++;
            } while (a[i] < pivot);

            do
            {
                j--;
            } while (a[j] > pivot);

            if (i >= j)
            {
                return j;
            }

            swap(a[i], a[j]);
        }
    }

    void quick_sort(vector<int> &a, int left, int right)
    {
        if (left >= right)
        {
            return;
        }

        int mid = partition(a, left, right);

        quick_sort(a, left, mid);
        quick_sort(a, mid + 1, right);
    }

public:
    std::vector<int> mySort(std::vector<int> &nums)
    {
        // 这里填写你的代码

        // 1. 快速排序
        vector<int> sorted = nums;
        if (!sorted.empty())
        {
            quick_sort(sorted, 0, (int)sorted.size() - 1);
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