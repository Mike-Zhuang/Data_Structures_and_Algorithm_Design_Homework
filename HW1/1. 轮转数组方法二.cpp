// 这是力扣算法，太聪明了！
#include <iostream>
#include <algorithm>
using namespace std;

int gcd(int a, int b) // 求最大公约数
{
    return b == 0 ? a : gcd(b, a % b);
}

int main()
{
    int n, k;
    cin >> n >> k;

    int nums[n];
    for (int i = 0; i < n; i++)
        cin >> nums[i];

    k = k % n;

    // 利用gcd确定循环个数，然后依次处理每个循环
    int count = gcd(k, n);
    for (int start = 0; start < count; ++start)
    {
        int current = start;
        int previous = nums[start];
        do
        {
            // 计算下一个交换位置
            int next = (current + k) % n;
            swap(nums[next], previous);
            current = next;
        } while (start != current); // 一直到起点
    }

    for (int i = 0; i < n; i++)
    {
        if (i)
            cout << " ";
        cout << nums[i];
    }
    cout << endl;
    return 0;
}
