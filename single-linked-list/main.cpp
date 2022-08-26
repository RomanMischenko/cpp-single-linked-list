#include "test.h"
#include "single-linked-list.h"

#include <iostream>

int main() {
    using namespace std;
    
    RunTest();

    SingleLinkedList<int> list{1, 2, 3, 4};

    for (auto& i : list) {
        cout << i << ": " << &i << endl;
    }
    
    list.PopFront();

    cout << "---after_del---"s << endl;

    for (auto& i : list) {
        cout << i << ": " << &i << endl;
    }

    cout << "---insert_after---"s << endl;
    auto it_1 = list.InsertAfter(list.begin(), 555);
    auto it_2 = list.InsertAfter(it_1, 666);
    list.InsertAfter(list.before_begin(), 111);
    for (auto& i : list) {
        cout << i << ": " << &i << endl;
    }
    cout << "it_1: "s << *it_1 << endl;
    cout << "it_2: "s << *it_2 << endl;

    cout << "---erase_after---"s << endl;
    auto it_3 = list.EraseAfter(list.begin());
    for (auto& i : list) {
        cout << i << ": " << &i << endl;
    }
    cout << "it_3: "s << *it_3 << endl;
    auto it_4 = list.EraseAfter(list.before_begin());
    for (auto& i : list) {
        cout << i << ": " << &i << endl;
    }
    cout << "it_4: "s << *it_4 << endl;

}