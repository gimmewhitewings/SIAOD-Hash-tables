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

    int firstHash(int key, unsigned size);

    static int secondHash(int key, unsigned size);

    void resizeTable();

    static unsigned getPrimeNumber(unsigned number);

public:
    HashTable(unsigned size);

    void insertElement(libraryCard *libraryCard);

    HashTableRow *getElement(int key);

    void deleteElement(int key);

    void printTable();

    unsigned getSize();
};

// folding hash
int HashTable::firstHash(int key, unsigned size) {
    int hash = 0;
    int keyLength = 0;
    int keyCopy = key;
    while (keyCopy > 0) {
        keyCopy /= 10;
        keyLength++;
    }
    int *keyArray = new int[keyLength];
    for (int i = 0; i < keyLength; i++) {
        keyArray[i] = key % 10;
        key /= 10;
    }
    for (int i = 0; i < keyLength; i += 2) {
        if (i + 1 < keyLength)
            hash += keyArray[i] * 10 + keyArray[i + 1];
        else
            hash += keyArray[i];
    }
    delete[] keyArray;
    return hash % size;
}

// second hash
int HashTable::secondHash(int key, unsigned size) {
    return 1 + (key % (size - 1));
}

inline void HashTable::resizeTable() {
    HashTableRow *oldTable = table;
    unsigned oldSize = size;
    size = getPrimeNumber(size * 2);
    table = new HashTableRow[size];
    closedAddresses = 0;
    for (unsigned i = 0; i < oldSize; i++) {
        if (oldTable[i].keyInventoryNumber != 0) {
            libraryCard card;
            card.inventoryNumber = oldTable[i].keyInventoryNumber;
            card.number = oldTable[i].number;
            strcpy_s(card.issueDate, oldTable[i].issueDate);
            strcpy_s(card.returnDate, oldTable[i].returnDate);
            insertElement(&card);
        }
    }
    delete[] oldTable;
}

HashTable::HashTable(unsigned size) {
    this->size = getPrimeNumber(size);
    table = new HashTableRow[size];
}

void HashTable::insertElement(libraryCard *libraryCard) {
    HashTableRow *currentElement = new HashTableRow(
            libraryCard->inventoryNumber,
            libraryCard->number,
            libraryCard->issueDate,
            libraryCard->returnDate
    );

    int iters = 0;
    int key = currentElement->keyInventoryNumber;
    int index = firstHash(key, size);
    int second = secondHash(key, size);

    while (table[index].keyInventoryNumber > 0) {
        index = (index + second) % size;
        iters++;
    }

    table[index] = *currentElement;
    this->closedAddresses++;

    if (closedAddresses / (double) size > 0.75) {
        this->resizeTable();
    }
}

inline HashTableRow *HashTable::getElement(int key) {
    int index = firstHash(key, size);
    while (table[index].keyInventoryNumber > 0) {
        if (table[index].keyInventoryNumber == key) {
            return &table[index];
        } else {
            index = (index + secondHash(key, this->size)) % this->size;
        }
    }
    return nullptr;
}


inline void HashTable::printTable() {
    int indexFieldSize = to_string(size).length();
    string horizontalDelimeter = "";
    string verticalDelimeter = " | ";

    for (int i = 0; i < indexFieldSize + 55; i++) {
        horizontalDelimeter += "_";
    }

    cout << horizontalDelimeter << endl;
    cout << "|" << setw(indexFieldSize) << "index" << verticalDelimeter
         << setw(13) << "keyInventoryNumber" << verticalDelimeter
         << setw(5) << "number" << verticalDelimeter
         << setw(10) << "issueDate" << verticalDelimeter
         << setw(10) << "returnDate" << verticalDelimeter << endl;
    cout << horizontalDelimeter << endl;

    for (size_t i = 0; i < size; i++) {
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
    while (table[index].keyInventoryNumber != 0) {
        if (table[index].keyInventoryNumber == key) {
            table[index].keyInventoryNumber = -1;
            return;
        } else {
            index = (index + secondHash(key, this->size)) % this->size;
        }
    }
}

unsigned HashTable::getSize() {
    return size;
}

unsigned HashTable::getPrimeNumber(unsigned number) {
    unsigned primeNumber = number;
    bool isPrime = false;
    while (!isPrime) {
        primeNumber++;
        isPrime = true;
        for (unsigned i = 2; i < primeNumber; i++) {
            if (primeNumber % i == 0) {
                isPrime = false;
                break;
            }
        }
    }
    return primeNumber;
}
