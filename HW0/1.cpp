#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    double total = 0.0, income;

    for (int i = 0; i < 12; ++i) {
        cin >> income;
        total += income;
    }

    double average = total / 12;

    cout << "¥" << fixed << setprecision(2) << average << endl;

    return 0;
}
