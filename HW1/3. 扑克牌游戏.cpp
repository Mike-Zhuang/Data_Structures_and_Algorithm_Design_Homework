#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

struct Card
{
    char flower[100];
    char number[100];
};

struct CardList
{
    Card *elem; // 存储基址
    int len;    // 当前牌数
    int size;   // 内存容量
    int inc;    // 扩容增量
};

void InitList(CardList &L, int initial_size, int inc_size)
{
    L.elem = (Card *)malloc(initial_size * sizeof(Card));
    if (!L.elem)
    {
        exit(1);
    }
    L.len = 0;
    L.size = initial_size;
    L.inc = inc_size;
}

// 众所周知，A=1,J=11,Q=12,K=13
int number(char *number)
{
    if (strcmp(number, "A") == 0)
    {
        return 1;
    }
    else if (strcmp(number, "J") == 0)
    {
        return 11;
    }
    else if (strcmp(number, "Q") == 0)
    {
        return 12;
    }
    else if (strcmp(number, "K") == 0)
    {
        return 13;
    }
    return atoi(number); // ASCII to Integer(这个不会！)
}

int main()
{
    CardList CardList;
    InitList(CardList, 100, 100);

    int n;
    if (!(cin >> n))
    {
        return 0;
    }

    char command[20];
    for (int i = 0; i < n; i++)
    {
        cin >> command;
        if (strcmp(command, "Append") == 0)
        {
            Card newCard;
            cin >> newCard.flower >> newCard.number;
            if (CardList.len >= CardList.size) // 内存不够，扩容
            {
                Card *newbase = (Card *)realloc(CardList.elem, (CardList.size + CardList.inc) * sizeof(Card));
                if (!newbase)
                {
                    exit(1);
                }
                CardList.elem = newbase;
                CardList.size += CardList.inc;
            }
            CardList.elem[CardList.len++] = newCard;
        }
        else if (strcmp(command, "Pop") == 0)
        {
            if (CardList.len == 0)
            {
                cout << "NULL" << endl;
            }
            else
            {
                cout << CardList.elem[0].flower << " " << CardList.elem[0].number << endl;
                memmove(CardList.elem, CardList.elem + 1, (CardList.len - 1) * sizeof(Card));
                CardList.len--;
            }
        }
        else if (strcmp(command, "Revert") == 0)
        {
            Card temp;
            for (int i = 0; i < CardList.len / 2; i++)
            {
                temp = CardList.elem[i];
                CardList.elem[i] = CardList.elem[CardList.len - 1 - i];
                CardList.elem[CardList.len - 1 - i] = temp;
            }
        }
        else if (strcmp(command, "Extract") == 0)
        {
            char goal[100];
            cin >> goal;

            Card Extracted[10000];
            int out = 0; // 抽出来的
            int in = 0;  // 没抽出来的
            // 匹配拿出来，不匹配留下来而且往前推
            for (int i = 0; i < CardList.len; i++)
            {
                if (strcmp(CardList.elem[i].flower, goal) == 0)
                {
                    Extracted[out++] = CardList.elem[i];
                }
                else
                {
                    CardList.elem[in++] = CardList.elem[i];
                }
            }
            CardList.len = in; // 原来的缩小

            if (out > 0)
            { // 冒泡排序
                for (int i = 0; i < out - 1; i++)
                {
                    for (int j = 0; j < out - 1 - i; j++)
                    {
                        if (number(Extracted[j].number) > number(Extracted[j + 1].number))
                        {
                            Card temp = Extracted[j];
                            Extracted[j] = Extracted[j + 1];
                            Extracted[j + 1] = temp;
                        }
                    }
                }

                if (CardList.len > 0)
                {
                    memmove(CardList.elem + out, CardList.elem, CardList.len * sizeof(Card));
                }

                memcpy(CardList.elem, Extracted, out * sizeof(Card));

                CardList.len += out;
            }
        }
    }

    if ((CardList.len) == 0)
    {
        cout << "NULL" << endl;
    }
    else
    {
        for (int i = 0; i < CardList.len; i++)
        {
            cout << CardList.elem[i].flower << " " << CardList.elem[i].number << endl;
        }
    }

    free(CardList.elem);
    return 0;
}