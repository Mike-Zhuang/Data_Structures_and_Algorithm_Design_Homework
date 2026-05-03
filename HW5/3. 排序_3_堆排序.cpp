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
    void sift_down(vector<int> &a, int start, int end)
    {
        int root = start;
        while (true)
        {
            int left_child = root * 2 + 1;
            if (left_child > end)
            {
                break;
            }

            int right_child = left_child + 1;
            int child = left_child;
            if (right_child <= end && a[right_child] > a[left_child])
            {
                child = right_child;
            }

            if (a[root] >= a[child])
            {
                break;
            }

            swap(a[root], a[child]);
            root = child;
        }
    }

    void heap_sort(vector<int> &a)
    {
        int n = (int)a.size();

        for (int start = n / 2 - 1; start >= 0; start--)
        {
            sift_down(a, start, n - 1);
        }

        for (int end = n - 1; end > 0; end--)
        {
            swap(a[0], a[end]);
            sift_down(a, 0, end - 1);
        }
    }

public:
    std::vector<int> mySort(std::vector<int> &nums)
    {
        // 这里填写你的代码

        // 3. 堆排序
        vector<int> sorted = nums;
        if (!sorted.empty())
        {
            heap_sort(sorted);
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