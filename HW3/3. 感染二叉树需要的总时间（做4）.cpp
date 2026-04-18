#include <iostream>
#include <vector>
#include <queue>
using namespace std;

vector<int> connect[100005];
bool visited[100005];

int main()
{
    int n, start;
    if (!(cin >> n >> start))
    {
        return 0;
    }

    for (int i = 0; i < n; i++)
    {
        int lc, rc;
        cin >> lc >> rc;

        if (lc != -1)
        {
            connect[i].push_back(lc); // i 可以感染 lc
            connect[lc].push_back(i); // lc 也可以感染 i (逆向传播)
        }

        if (rc != -1)
        {
            connect[i].push_back(rc);
            connect[rc].push_back(i);
        }
    }

    // start初始化
    queue<int> q;
    q.push(start);
    visited[start] = true;
    int minutes = 0;

    // 只要还有节点在散播病毒，就继续
    while (!q.empty())
    {
        // 当前这一分钟有几个节点同时在向外放毒
        int current_layer_size = q.size();

        // 当前这一分钟内的所有节点
        for (int i = 0; i < current_layer_size; i++)
        {
            int current = q.front();
            q.pop();
            for (int neighbor : connect[current])
            {
                if (!visited[neighbor])
                {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }

        // 当前这一分钟毒放完,如果队列里有了新节点,蔓延了，时间+1
        if (!q.empty())
        {
            minutes++;
        }
    }

    cout << minutes << endl;

    return 0;
}