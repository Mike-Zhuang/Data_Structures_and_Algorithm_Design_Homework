#include <iostream>
#include <queue>
#include <string>

using namespace std;

void dequeue(queue<int> &q)
{
    if (!q.empty())
    {
        cout << q.front() << endl;
        q.pop();
    }
    else
    {
        cout << "Queue is Empty" << endl;
    }
}

void enqueue(queue<int> &q, int num, int max_num)
{
    if (q.size() == max_num)
    {
        cout << "Queue is Full" << endl;
    }
    else
    {
        q.push(num);
    }
}

void max(queue<int> &q)
{
    if (q.empty())
    {
        cout << "Queue is Empty" << endl;
    }
    else
    {
        int max = q.front();
        int n = q.size();
        for (int i = 0; i < n; ++i)
        {
            int current = q.front();
            q.pop();

            if (current > max)
            {
                max = current;
            }

            q.push(current);
        }
        cout << max << endl;
    }
}

void quit(queue<int> &q)
{
    int n = q.size();
    for (int i = 0; i < n; ++i)
    {
        int current = q.front();
        cout << current << " ";
        q.pop();
    }
    cout << endl;
}
int main()
{
    int max_num;
    cin >> max_num;
    queue<int> q;
    string command;

    while (cin >> command)
    {
        if (command == "dequeue")
        {
            dequeue(q);
        }
        else if (command == "enqueue")
        {
            int num;
            cin >> num;
            enqueue(q, num, max_num);
        }
        else if (command == "max")
        {
            max(q);
        }
        else if (command == "quit")
        {
            quit(q);
            exit(0);
        }
    }
    return 0;
}