#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

struct ElemType
{
    char ID[100];
    char name[100];
};

struct StdList
{
    ElemType *elem; // 存储空间基址
    int len;        // 当前长度
    int size;       // 当前分配的存储容量
    int inc;        // 扩容增量
};

void Initlist(StdList &L, int initial_size, int inc_size)
{
    L.elem = (ElemType *)malloc(initial_size * sizeof(ElemType));
    if (!L.elem)
        exit(1);
    L.len = 0;
    L.size = initial_size;
    L.inc = inc_size;
}

bool ListInsert(StdList &L, int i, ElemType e)
{
    if (i < 1 || i > L.len + 1)
    {
        return false;
    }

    if (L.len >= L.size)
    {
        L.elem = (ElemType *)realloc(L.elem, (L.size + L.inc) * sizeof(ElemType));
        if (!L.elem)
            exit(1);
        L.size += L.inc;
    }

    memmove(L.elem + i, L.elem + i - 1, (L.len - i + 1) * sizeof(ElemType));
    L.elem[i - 1] = e;
    L.len++;
    return true;
}

bool ListDelete(StdList &L, int i)
{
    if (i < 1 || i > L.len)
    {
        return false;
    }

    ElemType *p = &(L.elem[i - 1]);
    memmove(p, p + 1, (L.len - i) * sizeof(ElemType));
    L.len--;
    return true;
}
int main()
{
    StdList List;
    Initlist(List, 10000, 1000);

    int n;
    if (!(cin >> n))
    {
        return 0;
    }

    for (int i = 0; i < n; i++)
    {
        cin >> List.elem[i].ID >> List.elem[i].name;
        List.len++;
    }

    char command[100];
    while (cin >> command)
    {
        if (strcmp(command, "insert") == 0)
        {
            int i;
            ElemType newstd;
            cin >> i >> newstd.ID >> newstd.name;
            if (ListInsert(List, i, newstd))
            {
                cout << "0" << endl;
            }
            else
            {
                cout << "-1" << endl;
            }
        }
        else if (strcmp(command, "remove") == 0)
        {
            int i;
            cin >> i;
            if (ListDelete(List, i))
            {
                cout << "0" << endl;
            }
            else
            {
                cout << "-1" << endl;
            }
        }
        else if (strcmp(command, "check") == 0)
        {
            bool found = false;
            char want[10];
            cin >> want;
            if (strcmp(want, "name") == 0)
            {
                char name[10000];
                cin >> name;
                for (int i = 0; i < List.len; i++)
                {
                    if (strcmp(List.elem[i].name, name) == 0)
                    {
                        cout << i + 1 << " " << List.elem[i].ID << " " << List.elem[i].name << endl;
                        found = true;
                        break;
                    }
                }
            }
            else if (strcmp(want, "no") == 0)
            {
                char no[10000];
                cin >> no;
                for (int i = 0; i < List.len; i++)
                {
                    if (strcmp(List.elem[i].ID, no) == 0)
                    {
                        cout << i + 1 << " " << List.elem[i].ID << " " << List.elem[i].name << endl;
                        found = true;
                        break;
                    }
                }
            }

            if (!found)
            {
                cout << "-1" << endl;
            }
        }
        else if (strcmp(command, "end") == 0)
        {
            cout << List.len << endl;
            break;
        }
    }
    free(List.elem);
    return 0;
}