# 作业 HW3 实验报告（二叉树的应用实验（哈夫曼编码））

姓名：庄程博  学号：2552870  日期：2026年4月5日

## 1. 涉及数据结构和相关背景

本实验围绕最优二叉树（Huffman Tree）展开。核心思想是按字符出现频率构造带权路径长度最小的二叉树，再据此生成变长前缀码，实现无损压缩。

由于字符集固定在 ASCII 范围内，字符种类上界明确（128 个），因此采用“数组 + 下标映射”管理树结构和辅助信息即可，既直接又便于调试。

## 2. 实验内容

### 2.1 哈夫曼编码和译码

#### 2.1.1 问题描述

对输入 ASCII 文本进行哈夫曼压缩，输出压缩后二进制编码串，并可将其解压回原文。

#### 2.1.2 基本要求

1. 能完成文本压缩。
2. 能打印压缩后编码结果。
3. 能完成解压并恢复原文本。
4. 程序具备基本健壮性和操作提示。

#### 2.1.3 数据结构设计

实验要求给定结点结构，不允许直接在结构体中增加权重和父节点字段：

```cpp
struct Node {
    int leftChild;
    int rightChild;
} tree[256];

struct NodeV {
    int leftChild;
    int rightChild;
    char c;
} decode_tree[256];
```

为满足建树和编码逆推需求，引入平行数组：

```cpp
int weight[256];      // 与 tree 下标一一对应，记录权重
int parent_node[256]; // 与 tree 下标一一对应，记录父节点
string huffman_code[256];
```

这种设计保持了接口协议不变，同时补足了算法所需状态。

#### 2.1.4 功能说明（函数）

```cpp
void init_tree();
```
初始化树、解码树、权重数组、父节点数组和编码表。

```cpp
void find_min(int n, int &min1, int &min2);
```
在 `[0, n)` 范围内选出两个“无父节点且权重最小”的结点，供建树合并使用。

```cpp
void build_huffman_tree(const string &text);
```
先统计频率，再反复合并最小权重结点完成建树，最后生成各字符的哈夫曼编码。

```cpp
string compress(const string &text);
```
根据 `huffman_code` 把原文本映射为 01 编码串。

```cpp
string decompress(const string &compressed);
```
从根节点按位遍历哈夫曼树，走到叶子就输出字符并回到根节点继续。

#### 2.1.5 调试分析（遇到的问题和解决方法）

##### 问题一：结构体字段受限，建树信息不全

现象：
实验给定的 `Node/NodeV` 只有左右孩子（以及解码字符），但哈夫曼建树至少还需要“权重 + 父节点”。缺少这两类信息时，既无法稳定完成“选最小二结点合并”，也无法从叶子逆推编码。

排错逻辑：
1. 首先确认约束边界：题目已明确结构体定义，直接修改结构体属于违背接口协议，不可取。
2. 分析算法最小信息闭环：
   - 合并阶段必须知道每个候选结点权重。
   - 生成编码阶段必须从叶子追溯到根，必须知道父节点。
3. 尝试替代方案评估：
    - 若每次都通过遍历树反求父节点，复杂度会明显上升，实现也更容易出错。
    - 最终采用“平行数组”方案：保持 `tree[i]` 结构不变，用 `weight[i]`、`parent_node[i]` 补足状态。

关键实现片段：

```cpp
// 平行两个数组存储权重和父节点
int weight[256];
int parent_node[256];

// 统计频率存入平行数组
for (char c : text)
{
    weight[(unsigned char)c]++; // 非负
    decode_tree[(unsigned char)c].c = c;
}

parent_node[m1] = current_nodes;
parent_node[m2] = current_nodes;
weight[current_nodes] = weight[m1] + weight[m2];
```

说明：
1. 通过下标一致性把 `tree[i]` 与 `weight[i]`、`parent_node[i]` 绑定，补足结构体中缺失的权重与父节点信息。
2. 频率统计只更新数组，不改动题目规定的结构体字段。
3. 合并阶段同步维护父子关系与累计权重，保证后续“从叶到根”逆推编码可行。

结果：
在不改动题目给定结构体的前提下，建树、编码生成、解码流程全部打通。这个方案本质上是“接口不变前提下的数据外置扩展”，工程可复用性较好。

##### 问题二：双重 reverse 导致前缀码失效

现象：
曾出现“压缩能生成 01 串，但解压得到乱码或错位字符串”的问题。典型表现是字符集合接近，但顺序和边界错误，说明码流可走通而码字定义已损坏。

排错逻辑：
1. 先验证树结构：逐步检查 `leftChild/rightChild` 与 `parent_node`，确认建树无断链。
2. 再验证码字生成方向：叶子到根追溯时，每上升一层应在码字前端补位（左 0 右 1）。
3. 复核字符串处理流程时发现历史错误：
   - 已经使用前插 `code = "0" + code` / `"1" + code`，得到的是“根到叶”的正确顺序。
   - 末尾又额外 `reverse(code.begin(), code.end())`，造成二次翻转。
4. 二次翻转会把本应前缀无歧义的码字映射到错误路径，破坏解码时“遇叶即停”的唯一性。

错误示意片段（历史版本）：

```cpp
while (p != -1) {
    if (tree[p].leftChild == current) code = "0" + code;
    else code = "1" + code;
    current = p;
    p = parent_node[current];
}
reverse(code.begin(), code.end());
```

说明：这里的 `reverse` 在该写法下会造成二次翻转，最终破坏码字方向。

修复后逻辑：

```cpp
while (p != -1) {
    if (tree[p].leftChild == current) code = "0" + code;
    else code = "1" + code;
    current = p;
    p = parent_node[current];
}
```

说明：修复方式是删除 `reverse`，保持“前插构造”的根到叶顺序不变。

结果：
修复后压缩与解压重新一致，码字恢复前缀码性质，解码路径和叶子边界都稳定下来。

#### 2.1.6 总结和体会

本题的主要算法复杂度可分阶段分析。设输入文本长度为 $m$，不同字符数为 $n$（本题 $n \le 128$），压缩后比特总长度为 $B$。

1. 频率统计阶段

遍历一次原文本，时间复杂度为 $O(m)$。

2. 建树阶段（核心）

数组实现下，每轮合并前都调用 `find_min` 线性扫描当前结点区间。若初始有 $n$ 个叶子结点，共进行 $n-1$ 轮合并。

第 $k$ 轮（$k=0,1,...,n-2$）扫描长度约为 $n+k$，因此

$$
T_{build} = \sum_{k=0}^{n-2}(n+k)
= n(n-1) + \frac{(n-2)(n-1)}{2}
= \frac{3n^2-5n+2}{2}
= O(n^2)
$$

这一步是本实现的主导项。

3. 编码表生成阶段

对每个出现过的字符，从叶子沿父指针回溯到根。若第 $i$ 个字符码长为 $d_i$，则总代价为 $\sum d_i$；最坏情况下可写为 $O(n^2)$，在较平衡树形下通常接近 $O(n\log n)$。

4. 压缩与解压阶段

压缩按字符查表并输出码字，按输出比特规模计为 $O(B)$；解压按位在树上游走，同样为 $O(B)$。

综上，总时间复杂度可写为

$$
T = O(m) + O(n^2) + O(B)
$$

在本题约束下，$n \le 128$ 为小常数，$O(n^2)$ 的数组扫描方案在实际运行中开销可控，且实现更直接：

1. 不需要堆结构和额外动态内存管理，实现复杂度低。
2. 连续数组访问具有良好缓存局部性，调试成本更小。
3. 对课程实验规模来说，代码可读性与稳定性收益高于引入复杂数据结构带来的理论优化。

## 3. 实验总结

本实验完成了 ASCII 文本的哈夫曼压缩与解压流程，实现了从频率统计、建树、编码到译码的完整闭环。调试过程中重点解决了“结构体受限下的信息补全”和“编码方向错误导致前缀码失效”两类关键问题。最终程序在功能正确性和实现可维护性之间达到了较好的平衡。

---

## 附录：完整代码

```cpp
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
```
