#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

struct BiTNode
{
    char data;
    int left;
    int right;
};

BiTNode BT1[10105], BT2[10105];

// 返回某个节点有没有父亲，如果有赋值true，否则赋值false，最后输出那个赋值为false的作为root
int MakeBiTree(BiTNode BT[])
{
    int n;
    cin >> n;

    if (n == 0)
    {
        return -1;
    }

    bool checkfather[10105] = {false};
    string lc, rc;
    for (int i = 0; i < n; i++)
    {
        cin >> BT[i].data >> lc >> rc;

        if (lc != "-")
        {
            BT[i].left = stoi(lc);
            checkfather[stoi(lc)] = true; // 编号为lc的节点有父亲了
        }
        else
        {
            BT[i].left = -1;
        }

        if (rc != "-")
        {
            BT[i].right = stoi(rc);
            checkfather[stoi(rc)] = true; // 编号为rc的节点有父亲了
        }
        else
        {
            BT[i].right = -1;
        }
    }

    int root = -1;
    for (int i = 0; i < n; i++)
    {
        if (!checkfather[i])
        {
            root = i;
            break;
        }
    }

    return root;
}

bool isIso(int R1, int R2)
{

    if (R1 == -1 && R2 == -1)
    {
        return true;
    }

    if ((R1 == -1 && R2 != -1) || (R1 != -1 && R2 == -1))
    {
        return false;
    }

    if (BT1[R1].data != BT2[R2].data)
    {
        return false;
    }

    bool no_swap = isIso(BT1[R1].left, BT2[R2].left) && isIso(BT1[R1].right, BT2[R2].right);
    bool swap = isIso(BT1[R1].left, BT2[R2].right) && isIso(BT1[R1].right, BT2[R2].left);

    return no_swap || swap;
}

int getDepth(int root, BiTNode BT[])
{

    if (root == -1)
    {
        return 0;
    }

    int left_depth = getDepth(BT[root].left, BT);
    int right_depth = getDepth(BT[root].right, BT);

    return max(left_depth, right_depth) + 1;
}

int main()
{
    int R1 = MakeBiTree(BT1);
    int R2 = MakeBiTree(BT2);

    if (isIso(R1, R2))
    {
        cout << "Yes" << endl;
    }
    else
    {
        cout << "No" << endl;
    }

    cout << getDepth(R1, BT1) << endl;
    cout << getDepth(R2, BT2) << endl;

    return 0;
}