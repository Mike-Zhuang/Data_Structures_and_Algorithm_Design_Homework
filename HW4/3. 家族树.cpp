#include <iostream>
#include <string>
#include <map>

using namespace std;

const int MAXN = 10010;

int parent_id[MAXN];
int depth_track[MAXN];

void solve()
{
    int n, m;

    while (cin >> n >> m && (n != 0 || m != 0))
    {
        map<string, int> name_to_id;
        string line;

        getline(cin, line);

        for (int i = 1; i <= n; i++)
        {
            getline(cin, line);

            int space_count = 0;
            while (space_count < line.length() && line[space_count] == ' ')
            {
                space_count++;
            }

            string name = line.substr(space_count);
            name_to_id[name] = i; // 名字分配ID

            // 确立父子关系
            if (space_count == 0)
            {
                parent_id[i] = -1; // 无缩进--牢祖宗
            }
            else
            {
                parent_id[i] = depth_track[space_count - 1]; // 有缩进--有粑粑
            }

            depth_track[space_count] = i;
        }

        // 查询
        for (int i = 0; i < m; i++)
        {
            string s1, s2, s3, s4, s5, s6;
            cin >> s1 >> s2 >> s3 >> s4 >> s5 >> s6;

            s6.pop_back(); // sb句号给我滚

            int u = name_to_id[s1];
            int v = name_to_id[s6];
            bool is_true = false;

            if (s4 == "child")
            {
                is_true = (parent_id[u] == v);
            }
            else if (s4 == "parent")
            {
                is_true = (parent_id[v] == u);
            }
            else if (s4 == "sibling")
            {
                is_true = (parent_id[u] == parent_id[v]);
            }
            else if (s4 == "descendant")
            {
                int current = u;
                while (current != -1)
                {
                    if (current == v)
                    {
                        is_true = true;
                        break;
                    }
                    current = parent_id[current];
                }
            }
            else if (s4 == "ancestor")
            {
                int current = v;
                while (current != -1)
                {
                    if (current == u)
                    {
                        is_true = true;
                        break;
                    }
                    current = parent_id[current];
                }
            }
            if (is_true)
            {
                cout << "True\n";
            }
            else
            {
                cout << "False\n";
            }
        }

        cout << endl;
    }
}

int main()
{
    solve();
    return 0;
}