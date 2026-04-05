#include <iostream>
#include <string>
#include <cmath>

using namespace std;

const int INF = pow(2, 31) - 1000;

struct Node
{
    int leftChild;
    int rightChild;
} tree[256];

struct NodeV
{
    int leftChild;
    int rightChild;
    char c;
} decode_tree[256];

// 平行两个数组存储权重和父节点
int weight[256];
int parent_node[256];

string huffman_code[256];
int root = -1;           // 根节点下标
int current_nodes = 128; // ascii有128个字符，合并产生的新节点从128开始存

void init_tree()
{
    for (int i = 0; i < 256; i++)
    {
        tree[i].leftChild = -1;
        tree[i].rightChild = -1;

        decode_tree[i].leftChild = -1;
        decode_tree[i].rightChild = -1;
        decode_tree[i].c = '\0';

        weight[i] = 0;
        parent_node[i] = -1;
    }

    for (int i = 0; i < 128; i++)
    {
        huffman_code[i] = "";
    }

    root = -1;
}

// 遍历寻找两个权重最小的无父节点,传入数组有效长度和用于接受结果两个引用，最小min1, 次小min2
// curmin1, curmin2存目前最小
void find_min(int n, int &min1, int &min2)
{
    int curmin1 = INF, curmin2 = INF;
    min1 = -1;
    min2 = -1;

    for (int i = 0; i < n; i++)
    {
        if (weight[i] > 0 && parent_node[i] == -1)
        {
            if (weight[i] < curmin1)
            {
                curmin2 = curmin1;
                min2 = min1;
                curmin1 = weight[i];
                min1 = i;
            }
            else if (weight[i] < curmin2)
            {
                curmin2 = weight[i];
                min2 = i;
            }
        }
    }
}

void build_huffman_tree(const string &text)
{
    init_tree();

    // 统计频率存入平行数组
    for (char c : text)
    {
        weight[(unsigned char)c]++; // 非负
        decode_tree[(unsigned char)c].c = c;
    }

    int unique_chars = 0; // 有多少不同

    for (int i = 0; i < 256; i++)
    {
        if (weight[i] > 0)
        {
            unique_chars++;
        }
    }
    if (unique_chars == 0)
    {
        return;
    }

    // 合并建树
    current_nodes = 128;
    while (true)
    {
        int m1, m2;
        find_min(current_nodes, m1, m2);

        if (m1 == -1 || m2 == -1)
        {
            break;
        }

        parent_node[m1] = current_nodes;
        parent_node[m2] = current_nodes;
        weight[current_nodes] = weight[m1] + weight[m2];
        parent_node[current_nodes] = -1;

        tree[current_nodes].leftChild = m1;
        tree[current_nodes].rightChild = m2;

        decode_tree[current_nodes].leftChild = m1;
        decode_tree[current_nodes].rightChild = m2;

        current_nodes++;
    }

    root = current_nodes - 1;

    // 生成哈夫曼编码
    for (int i = 0; i < 128; i++)
    {
        if (weight[i] > 0)
        {
            string code = "";
            int current = i;
            int p = parent_node[current]; // 找爸爸

            while (p != -1)
            {
                if (tree[p].leftChild == current)
                {
                    code = "0" + code;
                }
                else
                {
                    code = "1" + code;
                }

                current = p;
                p = parent_node[current];
            }
            huffman_code[i] = code;
        }
    }
}

// 压缩
string compress(const string &text)
{
    if (root == -1)
    {
        return "";
    }

    string compressed = "";
    for (char c : text)
    {
        compressed = compressed + huffman_code[(unsigned char)c];
    }

    return compressed;
}

// 解压
string decompress(const string &compressed)
{
    if (root == -1)
    {
        return "";
    }
    string output = "";
    int current = root;

    for (char bit : compressed) // 遍历压缩后的字符串
    {
        if (bit == '0')
        {
            current = decode_tree[current].leftChild;
        }
        else if (bit == '1')
        {
            current = decode_tree[current].rightChild;
        }

        // 到叶子
        if (decode_tree[current].leftChild == -1 && decode_tree[current].rightChild == -1)
        {
            output = output + decode_tree[current].c; // 读取记录在结构体里面的字符
            current = root;                           // 回到根节点准备下一个
        }
    }
    return output;
}

int main()
{
    string input;
    cout << "请输入字符串：";
    cin >> input;

    build_huffman_tree(input);
    string compressed = compress(input);
    string decompressed = decompress(compressed);

    cout << "压缩后的二进制：" << compressed << endl;
    cout << "解压后的字符串：" << decompressed << endl;

    return 0;
}