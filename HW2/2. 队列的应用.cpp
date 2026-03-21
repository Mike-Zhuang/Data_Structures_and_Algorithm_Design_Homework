#include <iostream>
#include <queue>
#include <cstring>

using namespace std;

struct Point
{
    int x;
    int y;
};

int matrix[1005][1005];

int main()
{

    int i, j;
    while (cin >> i >> j)
    {
        int zone = 0;
        int dx[4] = {0, 0, 1, -1};
        int dy[4] = {1, -1, 0, 0}; // 注意对应！
        for (int k = 0; k < i; k++)
        {
            for (int l = 0; l < j; l++)
            {
                cin >> matrix[k][l];
            }
        }

        for (int k = 0; k < i; k++)
        {
            for (int l = 0; l < j; l++)
            {
                if (matrix[k][l] == 1)
                {
                    queue<Point> point;
                    point.push({k, l});
                    matrix[k][l] = 0;
                    bool is_valid = true; // 没到边缘
                    while (!(point.empty()))
                    {
                        Point current = point.front();
                        point.pop();
                        if (current.x == 0 || current.x == i - 1 || current.y == 0 || current.y == j - 1)
                        {
                            is_valid = false;
                        }

                        for (int m = 0; m < 4; m++)
                        {
                            int new_x = current.x + dx[m];
                            int new_y = current.y + dy[m];

                            if (new_x >= 0 && new_x < i && new_y >= 0 && new_y < j && matrix[new_x][new_y] == 1)
                            {
                                point.push({new_x, new_y});
                                matrix[new_x][new_y] = 0;
                            }
                        }
                    }

                    if (is_valid == true)
                    {
                        zone++;
                    }
                }
            }
        }

        cout << zone << endl;
    }
    return 0;
}