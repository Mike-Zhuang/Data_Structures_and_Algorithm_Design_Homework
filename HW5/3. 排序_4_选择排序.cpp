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
public:
    std::vector<int> mySort(std::vector<int> &nums)
    {
        // 这里填写你的代码

        // 4. 选择排序
        vector<int> sorted = nums;
        int n = (int)sorted.size();

        for (int i = 0; i < n - 1; i++)
        {
            int min_index = i;
            for (int j = i + 1; j < n; j++)
            {
                if (sorted[j] < sorted[min_index])
                {
                    min_index = j;
                }
            }

            if (min_index != i)
            {
                swap(sorted[i], sorted[min_index]);
            }
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