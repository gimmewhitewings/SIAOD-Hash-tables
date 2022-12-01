#pragma once

#include "binWork.h"
#include "HashTable.h"
#include "HashBinary.h"

void testHashT() {
    HashTable *table = new HashTable(10);
    // insert element
    cout << "Insert element with key '1'" << endl;
    LibraryCard firstCard = LibraryCard(1, 1, "01.01.2001", "01.01.2001");
    table->insertElement(&firstCard);
    table->printTable();
    cout << endl << endl;
    // insert element with the same key
    cout << "Insert element with key '1' too" << endl;
    LibraryCard secondCard = LibraryCard(1, 2, "01.01.2001", "01.01.2001");
    table->insertElement(&secondCard);
    table->printTable();
    cout << endl << endl;
    // insert 10 elements for resize
    cout << "Insert 9 elements for resize" << endl;
    for (int i = 2; i < 11; i++) {
        LibraryCard newCard = LibraryCard(i, i, "01.01.2001", "01.01.2001");
        table->insertElement(&newCard);
    }
    table->printTable();
    cout << endl << endl;
    // delete element
    cout << "Delete element with key '5'" << endl;
    table->deleteElement(5);
    table->printTable();
    cout << endl << endl;
    // find element
    cout << "Find element with key '2'" << endl;
    HashTableRow *row = table->getElement(2);
    if (row) {
        cout << "Found item with key '2'" << endl;
        cout << "Key: " << row->keyInventoryNumber << endl;
        cout << "Number: " << row->number << endl;
        cout << "Issue date: " << row->issueDate << endl;
        cout << "Return date: " << row->returnDate << endl;
    } else {
        cout << "Element with key '2' not found" << endl;
    }
    cout << endl << endl;
}

void testBinF() {
    // create bin file from txt file
    cout << "Create bin file from txt file" << endl;
    txtToBin("test.txt", "test.dat");
    printBinFile("test.dat");
    cout << endl << endl;
    // add new card to bin file
    cout << "Add new card to bin file" << endl;
    LibraryCard card = LibraryCard(11, 425, "01.01.2001", "01.01.2001");
    addCard("test.dat", card);
    printBinFile("test.dat");
    cout << endl << endl;
    // delete card from bin file by key
    cout << "Delete card from bin file by key" << endl;
    deleteCardByKey("test.dat", 1);
    printBinFile("test.dat");
    cout << endl << endl;
    // get card from bin file by number
    cout << "Get card from bin file by number" << endl;
    LibraryCard *cardPtr = getCardPtrByNumber("test.dat", 2);
    if (cardPtr) {
        cout << "Found card with number '2'" << endl;
        cout << "Key: " << cardPtr->inventoryNumber << endl;
        cout << "Number: " << cardPtr->bookNumber << endl;
        cout << "Issue date: " << cardPtr->issueDate << endl;
        cout << "Return date: " << cardPtr->returnDate << endl;
    } else {
        cout << "Card with number '2' not found" << endl;
    }
}

void testHashBinary() {
    HashTable *table = new HashTable(10);
    cout << "Generating bin file" << endl;
    generateBinFile("test.dat", 5);
    printBinFile("test.dat");
    // insert elements from bin file to hash table
    cout << "Insert elements from bin file to hash table" << endl;
    for (int i = 0; i < 5; ++i) {
        insertFromFile(table, "test.dat", i);
    }
    table->printTable();
    cout << endl << endl;
    // delete element
    cout << "Delete element with key '2'" << endl;
    deleteFromFileAndTable(table, "test.dat", 2);
    printBinFile("test.dat");
    table->printTable();
    cout << endl << endl;
    // find element
    cout << "Find element with key '5'" << endl;
    LibraryCard *card = findInFileWithHashTable(table, "test.dat", 5);
    if (card)
        cout << "Found item with key '5'" << endl;
    else
        cout << "Element with key '5' not found" << endl;
}

void generalTest() {
    HashTable *smallTable = new HashTable(10);
    int smallSize = 5;
    cout << "Generating bin file with size 5" << endl;
    generateBinFile("test.dat", smallSize);
    printBinFile("test.dat");
    // insert elements from bin file to hash table
    cout << "Insert elements from bin file to hash table" << endl;
    for (int i = 0; i < smallSize; ++i) {
        insertFromFile(smallTable, "test.dat", i);
    }
    smallTable->printTable();
    cout << endl << endl;

    delete smallTable;

    int bigSize = 300000;
    HashTable *bigTable = new HashTable(bigSize);
    cout << "Generating bin file with size 300000" << endl;
    generateBinFile("test.dat", bigSize);
    // insert elements from bin file to hash table
    cout << "Inserting elements from bin file to hash table..." << endl;
    for (int i = 1; i < bigSize; i++) {
        insertFromFile(bigTable, "test.dat", i);
    }
    cout << "Inserting elements from bin file to hash table finished" << endl;

    // find time of getting first element
    clock_t start = clock();
    findInFileWithHashTable(bigTable, "test.dat", 1);
    clock_t end = clock();
    double seconds = (double) (end - start) / CLOCKS_PER_SEC;
    cout << "First element search time: " << seconds << endl;

    // find time of getting last element
    start = clock();
    findInFileWithHashTable(bigTable, "test.dat", bigSize - 1);
    end = clock();
    seconds = (double) (end - start) / CLOCKS_PER_SEC;
    cout << "Last element search time: " << seconds << endl;

    // find time of getting middle element
    start = clock();
    findInFileWithHashTable(bigTable, "test.dat", bigSize / 2);
    end = clock();
    seconds = (double) (end - start) / CLOCKS_PER_SEC;
    cout << "Middle element search time: " << seconds << endl;
}