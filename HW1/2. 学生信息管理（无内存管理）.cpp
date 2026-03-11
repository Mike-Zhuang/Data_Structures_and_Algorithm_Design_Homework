#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct student
{
    string ID; // 改为 string，以正确处理所有学号情况（包括前导零等）
    string name;
};

int main()
{
    int n; // 学生人数
    cin >> n;
    vector<student> students(n);

    for (int i = 0; i < n; i++)
    {
        cin >> students[i].ID >> students[i].name;
    }

    string command;
    while (cin >> command)
    {
        if (command == "end")
        {
            cout << n << endl;
            break;
        }
        else if (command == "insert")
        {
            int position; // 位置
            cin >> position;
            if (position < 1 || position > n + 1)
            {
                cout << "-1" << endl;
                continue;
            }
            else
            {
                student new_student;
                cin >> new_student.ID >> new_student.name;
                students.insert(students.begin() + position - 1, new_student);
                n++;
                cout << "0" << endl;
            }
        }
        else if (command == "remove")
        {
            int position;
            cin >> position;
            if (position < 1 || position > n)
            {
                cout << "-1" << endl;
                continue;
            }
            else
            {
                students.erase(students.begin() + position - 1);
                n--;
                cout << "0" << endl;
            }
        }
        else if (command == "check")
        {
            string type;
            cin >> type;
            if (type == "name")
            {
                string name;
                cin >> name;
                bool found = false;
                for (int i = 0; i < n; i++)
                {
                    if (students[i].name == name)
                    {
                        cout << i + 1 << " " << students[i].ID << " " << students[i].name << endl;
                        found = true;
                        break;
                    }
                }
                if (!found)
                    cout << "-1" << endl;
            }
            else if (type == "no")
            {
                string ID;
                cin >> ID;
                bool found = false;
                for (int i = 0; i < n; i++)
                {
                    if (students[i].ID == ID)
                    {
                        cout << i + 1 << " " << students[i].ID << " " << students[i].name << endl;
                        found = true;
                        break;
                    }
                }
                if (!found)
                    cout << "-1" << endl;
            }
        }
    }
}