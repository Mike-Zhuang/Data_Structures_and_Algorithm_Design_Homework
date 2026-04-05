#include <iostream>
#include <string>
#include <stack>
#include <vector>

using namespace std;

struct TreeNode
{
    char data;
    TreeNode *left, *right;
    TreeNode(char data) : data(data), left(NULL), right(NULL) {}
};

vector<char> pre_order;
vector<char> in_order;

TreeNode *BuildTree(int pre_left, int pre_right, int in_left, int in_right)
{
    if (pre_left > pre_right)
    {
        return nullptr;
    }

    char root_val = pre_order[pre_left];

    TreeNode *root = new TreeNode(root_val);

    // 中序找根
    int root_index_in_inorder = in_left;
    while (root_index_in_inorder <= in_right && in_order[root_index_in_inorder] != root_val)
    {
        root_index_in_inorder++;
    }

    int left_len = root_index_in_inorder - in_left; // root-左边界=左子树长度

    root->left = BuildTree(pre_left + 1, pre_left + left_len, in_left, root_index_in_inorder - 1);
    root->right = BuildTree(pre_left + left_len + 1, pre_right, root_index_in_inorder + 1, in_right);

    return root;
}

void postOrder(TreeNode *root)
{
    if (root == nullptr)
    {
        return;
    }
    postOrder(root->left);
    postOrder(root->right);
    cout << root->data;
}

int main()
{
    int n;
    cin >> n;

    stack<char> node_stack;
    string command;
    char node;

    // 提取前序和中序（和之前一样）
    for (int i = 0; i < 2 * n; i++)
    {
        cin >> command;
        if (command == "push")
        {
            cin >> node;
            pre_order.push_back(node);
            node_stack.push(node);
        }
        else if (command == "pop")
        {
            in_order.push_back(node_stack.top());
            node_stack.pop();
        }
    }

    TreeNode *RealRoot = BuildTree(0, n - 1, 0, n - 1);

    postOrder(RealRoot);
    cout << endl;

    return 0;
}