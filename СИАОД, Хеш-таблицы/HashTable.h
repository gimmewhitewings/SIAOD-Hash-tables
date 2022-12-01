#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include "binWork.h"

using namespace std;


struct HashTableRow {
    int keyInventoryNumber;
    int number;
    char issueDate[11]{};
    char returnDate[11]{};

    HashTableRow(const int keyInventoryNumber,
                 const int number,
                 const char *issueDate,
                 const char *returnDate) {
        this->keyInventoryNumber = keyInventoryNumber;
        this->number = number;
        strcpy_s(this->issueDate, issueDate);
        strcpy_s(this->returnDate, returnDate);
    }

    HashTableRow() {
        this->keyInventoryNumber = 0;
        this->number = 0;
        this->issueDate[10] = '\0';
        this->returnDate[10] = '\0';
    }
};

struct HashTable {
private:
    unsigned size;
    unsigned closedAddresses = 0;
    HashTableRow *table;

    static int firstHash(int key, unsigned tableSize);

    static int secondHash(int key, unsigned size);

    void resizeTable();

    static unsigned getPrimeNumber(unsigned number);

public:
    HashTable(unsigned size);

    void insertElement(LibraryCard *libraryCard);

    HashTableRow *getElement(int key);

    void deleteElement(int key);

    void printTable();

};

// folding hash
int HashTable::firstHash(int key, unsigned tableSize) {
    int hash = 0;
    int keyLength = 0;
    int keyCopy = key;
    while (keyCopy > 0) { // get key length
        keyCopy /= 10;
        keyLength++;
    }
    int *keyArray = new int[keyLength]; // create array of key digits
    for (int i = 0; i < keyLength; i++) { // fill array of key digits
        keyArray[i] = key % 10; // get last digit
        key /= 10; // delete last digit
    }
    for (int i = 0; i < keyLength; i += 2) { // sum every second digit
        if (i + 1 < keyLength) // if there is a second digit
            hash += keyArray[i] * 10 + keyArray[i + 1]; // sum two digits
        else // if there is only one digit
            hash += keyArray[i]; // sum one digit
    }
    delete[] keyArray; // delete array of key digits
    return hash % tableSize; // return hash
}

// second hash
int HashTable::secondHash(int key, unsigned size) {
    return 1 + (key % (size - 1));
}

inline void HashTable::resizeTable() {
    HashTableRow *oldTable = table;
    unsigned oldSize = size;
    size = getPrimeNumber(size * 2); // get new size
    table = new HashTableRow[size];
    closedAddresses = 0;
    for (unsigned i = 0; i < oldSize; i++) {
        if (oldTable[i].keyInventoryNumber != 0) { // if cell is not empty
            LibraryCard card;
            card.inventoryNumber = oldTable[i].keyInventoryNumber;
            card.bookNumber = oldTable[i].number;
            strcpy_s(card.issueDate, oldTable[i].issueDate);
            strcpy_s(card.returnDate, oldTable[i].returnDate);
            insertElement(&card); // insert element to new table
        }
    }
    delete[] oldTable; // delete old table
}

HashTable::HashTable(unsigned size) {
    this->size = getPrimeNumber(size);
    table = new HashTableRow[size];
}

void HashTable::insertElement(LibraryCard *libraryCard) {
    HashTableRow *currentElement = new HashTableRow(
            libraryCard->inventoryNumber,
            libraryCard->bookNumber,
            libraryCard->issueDate,
            libraryCard->returnDate
    );

    int key = currentElement->keyInventoryNumber;
    int index = firstHash(key, size);
    int second = secondHash(key, size);

    while (table[index].keyInventoryNumber > 0) { // while cell is not empty
        index = (index + second) % size; // get next index
    }

    table[index] = *currentElement;
    this->closedAddresses++;

    if (closedAddresses / (double) size > 0.75) { // if table is full more than 75%
        this->resizeTable();
    }
}

inline HashTableRow *HashTable::getElement(int key) {
    int index = firstHash(key, size);
    int second = secondHash(key, size);
    while (table[index].keyInventoryNumber > 0) { // while cell is not empty
        if (table[index].keyInventoryNumber == key) { // if key is found
            return &table[index]; // return pointer to element
        } else { // if key is not found
            index = (index + second) % size; // get next index
        }
    }
    return nullptr; // if key is not found
}


inline void HashTable::printTable() {
    int indexFieldSize = to_string(size).length();
    string horizontalDelimeter;
    string verticalDelimeter = " | ";

    for (int i = 0; i < indexFieldSize + 55; i++) { // create horizontal delimeter
        horizontalDelimeter += "_";
    }

    cout << horizontalDelimeter << endl;
    cout << "|" << setw(indexFieldSize) << "index" << verticalDelimeter
         << setw(13) << "keyInventoryNumber" << verticalDelimeter
         << setw(5) << "number" << verticalDelimeter
         << setw(10) << "issueDate" << verticalDelimeter
         << setw(10) << "returnDate" << verticalDelimeter << endl;
    cout << horizontalDelimeter << endl;

    for (size_t i = 0; i < size; i++) { // print table
        cout << "| " << setw(indexFieldSize)
             << i + 1 << verticalDelimeter;
        if (table[i].keyInventoryNumber > 0) {
            cout << setw(13) << table[i].keyInventoryNumber << verticalDelimeter
                 << setw(5) << table[i].number << verticalDelimeter
                 << setw(10) << table[i].issueDate << verticalDelimeter
                 << setw(10) << table[i].returnDate;
        } else {
            cout << setw(13) << " " << verticalDelimeter
                 << setw(5) << " " << verticalDelimeter
                 << setw(10) << " " << verticalDelimeter
                 << setw(10) << " ";
        }
        cout << " |" << '\n' << horizontalDelimeter << '\n';
    }
}

void HashTable::deleteElement(int key) {
    int index = firstHash(key, this->size);
    int second = secondHash(key, this->size);
    while (table[index].keyInventoryNumber != 0) {
        if (table[index].keyInventoryNumber == key) {
            table[index].keyInventoryNumber = -1; // mark cell as deleted
            return;
        } else
            index = (index + second) % this->size; // get next index
    }
}

// get first prime number greater than number
unsigned HashTable::getPrimeNumber(unsigned number) {
    unsigned primeNumber = number;
    bool isPrime = false;
    while (!isPrime) {
        primeNumber++;
        isPrime = true;
        for (unsigned i = 2; i < (int) sqrt(primeNumber); i++) { // check if number is prime
            if (primeNumber % i == 0) {
                isPrime = false;
                break;
            }
        }
    }
    return primeNumber;
}
