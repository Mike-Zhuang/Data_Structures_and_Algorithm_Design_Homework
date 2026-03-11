// 力扣方法三，根本想不到
#include <iostream>
#include <algorithm>
using namespace std;

// 翻转数组的指定区间 [start, end]
void reverse(int nums[], int start, int end)
{
    while (start < end)
    {
        swap(nums[start], nums[end]);
        start++;
        end--;
    }
}

int main()
{
    int n, k;
    cin >> n >> k;

    int nums[n];
    for (int i = 0; i < n; i++)
    {
        cin >> nums[i];
    }

    k = k % n;

    reverse(nums, 0, n - 1); // 翻转整个数组
    reverse(nums, 0, k - 1); // 翻转前 k 个元素
    reverse(nums, k, n - 1); // 翻转后面的元素

    for (int i = 0; i < n; i++)
    {
        if (i)
            cout << " ";
        cout << nums[i];
    }
    cout << endl;
    return 0;
}
