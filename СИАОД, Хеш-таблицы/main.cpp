#include <iostream>
#include "Tests.h"


using namespace std;


int main() {
    // Menu for choosing a test
    int testNumber;
    while (true) {
        cout << "Choose a test" << endl;
        cout << "1. Hash table testing" << endl;
        cout << "2. Work with binary file testing" << endl;
        cout << "3. Work with both hash table and binary file" << endl;
        cout << "4. General test" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter a number: ";
        cin >> testNumber;
        switch (testNumber) {
            case 1:
                testHashTable();
                break;
            case 2:
                testBinWork();
                break;
            case 3:
                testHashBinary();
                break;
            case 4:
                generalTest();
                break;
            case 5:
                cout << "Goodbye!" << endl;
                return 0;
            default:
                cout << "Wrong number" << endl;
        }
        system("pause");
        system("cls");
    }
}

