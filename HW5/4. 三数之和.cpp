#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

class Solution
{
public:
    vector<vector<int>> three_sum(vector<int> &nums)
    {
        vector<vector<int>> ans;
        int n = (int)nums.size();

        sort(nums.begin(), nums.end());

        for (int first = 0; first < n; first++)
        {
            if (first > 0 && nums[first] == nums[first - 1])
            {
                continue;
            }

            int left = first + 1;
            int right = n - 1;

            while (left < right)
            {
                int sum = nums[first] + nums[left] + nums[right];

                if (sum == 0)
                {
                    ans.push_back({nums[first], nums[left], nums[right]});

                    left++;
                    right--;

                    while (left < right && nums[left] == nums[left - 1])
                    {
                        left++;
                    }

                    while (left < right && nums[right] == nums[right + 1])
                    {
                        right--;
                    }
                }
                else if (sum < 0)
                {
                    left++;
                }
                else
                {
                    right--;
                }
            }
        }

        return ans;
    }
};

int main()
{
    int n;
    cin >> n;

    vector<int> nums(n);
    for (int i = 0; i < n; i++)
    {
        cin >> nums[i];
    }

    Solution solution;
    vector<vector<int>> ans = solution.three_sum(nums);

    for (int i = 0; i < (int)ans.size(); i++)
    {
        cout << ans[i][0] << ' ' << ans[i][1] << ' ' << ans[i][2] << '\n';
    }

    return 0;
}
