#include <iostream>

using namespace std;

struct TreeNode
{
    int val;
    int count;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int v) : val(v), count(1), left(nullptr), right(nullptr) {}
};

class BST
{

private:
    TreeNode *root;

    // 递归插入
    TreeNode *insert(TreeNode *node, int x)
    {
        if (!node)
        {
            return new TreeNode(x);
        }
        if (x == node->val)
        {
            node->count++;
        }
        else if (x < node->val)
        {
            node->left = insert(node->left, x);
        }
        else
        {
            node->right = insert(node->right, x);
        }
        return node;
    }

    // 寻找右子树最小值节点
    TreeNode *findMinNode(TreeNode *node)
    {
        while (node && node->left)
        {
            node = node->left;
        }
        return node;
    }

    // 删除
    TreeNode *remove(TreeNode *node, int x, bool &deleted)
    {
        if (!node)
        {
            return nullptr;
        }

        if (x < node->val)
        {
            node->left = remove(node->left, x, deleted);
        }
        else if (x > node->val)
        {
            node->right = remove(node->right, x, deleted);
        }
        else
        {
            deleted = true;
            if (node->count > 1)
            {
                node->count--;
            }
            else
            {
                if (!node->left)
                {
                    TreeNode *temp = node->right;
                    delete node;
                    return temp;
                }
                else if (!node->right)
                {
                    TreeNode *temp = node->left;
                    delete node;
                    return temp;
                }

                TreeNode *temp = findMinNode(node->right);
                node->val = temp->val;
                node->count = temp->count;

                temp->count = 1;
                node->right = remove(node->right, temp->val, deleted);
            }
        }
        return node;
    }

public:
    BST() : root(nullptr) {}

    void insert(int x)
    {
        root = insert(root, x);
    }

    void remove(int x)
    {
        bool deleted = false;
        root = remove(root, x, deleted);
        if (!deleted)
        {
            cout << "None\n";
        }
    }

    void queryCount(int x)
    {
        TreeNode *current = root;
        while (current)
        {
            if (x == current->val)
            {
                cout << current->count << endl;
                return;
            }
            else if (x < current->val)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }
        }
        cout << "0\n";
    }

    void get_min()
    {
        if (!root)
        {
            return;
        }
        TreeNode *current = findMinNode(root);
        cout << current->val << endl;
    }

    void get_pre(int x)
    {
        TreeNode *current = root;
        TreeNode *pre = nullptr;

        while (current)
        {
            if (current->val < x)
            {
                pre = current;            // 有资格
                current = current->right; // 尝试找更大的
            }
            else
            {
                current = current->left;
            }
        }

        if (pre)
        {
            cout << pre->val << endl;
        }
        else
        {
            cout << "None\n";
        }
    }
};

int main()
{
    BST bst;
    int n;
    if (!(cin >> n))
    {
        return 0;
    }

    while (n--)
    {
        int command, x;
        cin >> command;
        switch (command)
        {
        case 1:
            cin >> x;
            bst.insert(x);
            break;
        case 2:
            cin >> x;
            bst.remove(x);
            break;
        case 3:
            cin >> x;
            bst.queryCount(x);
            break;
        case 4:
            bst.get_min();
            break;
        case 5:
            cin >> x;
            bst.get_pre(x);
            break;
        }
    }
    return 0;
}