#include <iostream>
#include <string>

using namespace std;

struct student
{
    string ID;
    string name;
};

struct SeqList
{
    student *data;
    int length;
    int capacity;
};