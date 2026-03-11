#include <iostream>
using namespace std;

void rotate(int k, int n)
{

    int table0[n];
    for (int i = 0; i < n; i++)
    {
        cin >> table0[i];
    }
    int table1[n];
    for (int i = 0; i < n; i++)
    {
        table1[(i + k) % n] = table0[i];
    }
    for (int i = 0; i < n; i++)
    {
        cout << table1[i] << " ";
    }
    cout << endl;
}

int main()
{
    int n, k;
    cin >> n >> k; // n是数组长度，k是向右轮转次数
    rotate(k, n);
    return 0;
}