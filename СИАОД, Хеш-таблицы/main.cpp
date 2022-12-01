#include <iostream>
#include "Tests.h"


using namespace std;


int main() {
    // Menu for choosing a test
    int testNumber;
    while (true) {
        cout << "Practical work #3 Povedenok Stanislav" << endl;
        cout << "-------------------------------------" << endl;
        cout << "Choose a test" << endl;
        cout << "1. Hash table testing" << endl;
        cout << "2. Work with binary file testing" << endl;
        cout << "3. Work with both hash table and binary file" << endl;
        cout << "4. General test" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter a number: ";
        cin >> testNumber;
        switch (testNumber) {
            case 1:
                testHashT();
                break;
            case 2:
                testBinF();
                break;
            case 3:
                testHashBinary();
                break;
            case 4:
                generalTest();
                break;
            case 0:
                cout << "Goodbye!" << endl;
                return 0;
            default:
                cout << "Wrong number, try again!" << endl;
        }
        system("pause");
        system("cls");
    }
}

