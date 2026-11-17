#include <iostream>
#include <algorithm>

int& highest(int marks[], int n) {
    int index = 0;
    for (int i = 1; i < n; ++i)
        if (marks[i] > marks[index]) index = i;
    return marks[index];
}
const int& highestReadOnly(const int marks[], int n) {
    int index = 0;
    for (int i = 1; i < n; ++i)
        if (marks[i] > marks[index]) index = i;
    return marks[index];
}
int highestCopy(const int marks[], int n) {
    int index = 0;
    for (int i = 1; i < n; ++i)
        if (marks[i] > marks[index]) index = i;
    return marks[index];
}

// Deliberately unsafe: do not call. Returning a reference to this local is undefined behaviour.
int& danglingReference() {
    int local = 99;
    return local;
}

int main() {
    int marks[] = {78, 91, 84, 88, 91};
    const int n = sizeof(marks) / sizeof(marks[0]);
    std::cout << "Highest before moderation: " << highest(marks, n) << '\n';
    highest(marks, n) += 2;
    std::cout << "Highest after +2: " << highest(marks, n) << '\n';
    std::cout << "Read-only highest: " << highestReadOnly(marks, n) << '\n';
    std::cout << "Copied highest: " << highestCopy(marks, n) << '\n';

    // Index alternative:
    int index = 0;
    for (int i = 1; i < n; ++i) if (marks[i] > marks[index]) index = i;
    marks[index] += 2;
    std::cout << "Index identifies element " << index << '\n';

    // Pointer alternative:
    int* p = &marks[index];
    std::cout << "Pointer value: " << *p << '\n';
}
