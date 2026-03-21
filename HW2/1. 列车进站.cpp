#include <iostream>
#include <stack>
#include <cstring>

using namespace std;

int main()
{
    char start[100]; // 列车初始状态
    if (!(cin >> start))
    {
        return 0;
    }
    char end[100]; // 列车最终状态
    while (cin >> end)
    {
        stack<char> station; // 站台
        char *i = start;
        char *j = end;
        while (*i != '\0')
        {
            station.push(*i);
            i++;

            while (!(station.empty()) && station.top() == *j)
            {
                station.pop();
                j++;
            }
        }
        if (station.empty())
        {
            cout << "yes" << endl;
        }
        else
        {
            cout << "no" << endl;
        }
    }
    return 0;
}