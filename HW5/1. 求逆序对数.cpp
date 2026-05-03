#include <iostream>

using namespace std;

void calculate(int group[], int n, int *count)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (group[j] < group[i])
            {
                (*count)++;
            }
        }
    }
}
int main()
{
    int n;
    int group[20005];
    int count = 0;
    while (cin >> n)
    {
        for (int i = 0; i < n; i++)
        {
            cin >> group[i];
        }
        if (n == 0)
        {
            exit(0);
        }
        else
        {
            calculate(group, n, &count);
            cout << count << endl;
            count = 0;
        }
    }
    return 0;
}