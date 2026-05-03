/**
 * @file    template.cpp
 * @name    模板程序
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
    std::string largestNumber(std::vector<int> &nums)
    {
        int n = nums.size();
        string ans_str = "";
        long long num = 0;
        string temp_num = "0";
        // 这里填写你的代码
        // for (int i = 0; i < n; i++)
        // {

        //     num = nums[i];
        //     temp_num = to_string(num);
        //     if ((temp_num + ans_str) >= (ans_str + temp_num))
        //     {
        //         ans_str = temp_num + ans_str;
        //     }
        //     else if ((temp_num + ans_str) < (ans_str + temp_num))
        //     {
        //         ans_str = ans_str + temp_num;
        //     }
        // }
        sort(nums.begin(), nums.end(), [](int x, int y)
             { return to_string(x) + to_string(y) > to_string(y) + to_string(x); });

        if (nums[0] == 0)
        {
            return "0";
        }

        for (int i = 0; i < n; i++)
        {
            temp_num = to_string(nums[i]);
            ans_str += temp_num;
        }
        return ans_str;
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
    std::cout << s.largestNumber(nums) << std::endl;
    return 0;
}