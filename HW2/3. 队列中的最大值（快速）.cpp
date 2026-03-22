#include <iostream>
#include <string>

using namespace std;

// 主队列和单调队列
struct MaxQueue
{
    int *q_arr;
    int q_head, q_tail, q_count;

    int *d_arr;
    int d_head, d_tail, d_count;

    int capacity;

    MaxQueue(int cap)
    {
        capacity = cap;
        // 划出两块连续的内存空间
        q_arr = new int[capacity > 0 ? capacity : 1]; // 防止 capacity 为 0 时数组大小问题
        d_arr = new int[capacity > 0 ? capacity : 1];

        q_head = q_tail = q_count = 0;
        d_head = d_tail = d_count = 0;
    }

    ~MaxQueue()
    {
        delete[] q_arr;
        delete[] d_arr;
    }
};

void dequeue(MaxQueue &q)
{

    if (q.q_count == 0)
    {
        cout << "Queue is Empty" << endl;
    }
    else
    {
        int val = q.q_arr[q.q_head];
        cout << val << endl;

        q.q_head = (q.q_head + 1) % q.capacity;
        q.q_count--;

        if (q.d_count > 0 && q.d_arr[q.d_head] == val)
        {

            q.d_head = (q.d_head + 1) % q.capacity;
            q.d_count--;
        }
    }
}

void enqueue(MaxQueue &q, int num)
{
    // 探满
    if (q.q_count == q.capacity)
    {
        cout << "Queue is Full" << endl;
    }
    else
    {
        // 主队列入队
        q.q_arr[q.q_tail] = num;
        q.q_tail = (q.q_tail + 1) % q.capacity;
        q.q_count++;

        // 单调队列入队前挤压
        while (q.d_count > 0)
        {
            // 获取前一个元素下标
            int back_idx = (q.d_tail - 1 + q.capacity) % q.capacity;

            if (q.d_arr[back_idx] < num)
            {
                // 回退尾指针
                q.d_tail = back_idx;
                q.d_count--;
            }
            else
            {
                // 停止挤压
                break;
            }
        }

        // 单调队列入队
        q.d_arr[q.d_tail] = num;
        q.d_tail = (q.d_tail + 1) % q.capacity;
        q.d_count++;
    }
}

void max(MaxQueue &q)
{
    // 判断主队列是否为空
    if (q.q_count == 0)
    {
        cout << "Queue is Empty" << endl;
    }
    else
    {
        // 从单调队列头部拿最大值
        cout << q.d_arr[q.d_head] << endl;
    }
}

void quit(MaxQueue &q)
{
    int curr = q.q_head;
    for (int i = 0; i < q.q_count; ++i)
    {
        cout << q.q_arr[curr] << " ";
        curr = (curr + 1) % q.capacity;
    }
    cout << endl;
}

int main()
{
    int max_num;
    cin >> max_num;

    MaxQueue q(max_num);
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
            enqueue(q, num);
        }
        else if (command == "max")
        {
            max(q);
        }
        else if (command == "quit")
        {
            quit(q);
            break; // MaxQueue自动执行析构函数释放内存
        }
    }
    return 0;
}
