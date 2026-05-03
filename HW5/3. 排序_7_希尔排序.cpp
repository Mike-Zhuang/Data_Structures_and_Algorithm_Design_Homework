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

        // 7. 希尔排序
        vector<int> sorted = nums;
        int n = (int)sorted.size();

        for (int gap = n / 2; gap > 0; gap /= 2)
        {
            for (int i = gap; i < n; i++)
            {
                int key = sorted[i];
                int j = i;

                while (j - gap >= 0 && sorted[j - gap] > key)
                {
                    sorted[j] = sorted[j - gap];
                    j -= gap;
                }
                sorted[j] = key;
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