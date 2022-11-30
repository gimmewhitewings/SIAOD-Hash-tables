#pragma once

#include "binWork.h"
#include "HashTable.h"
#include "HashBinary.h"

void testHashTable() {
    HashTable *table = new HashTable(10);
    // insert element
    LibraryCard card = LibraryCard(1, 1, "01.01.2001", "01.01.2001");
    table->insertElement(&card);
    // insert element with the same key
    LibraryCard card2 = LibraryCard(1, 2, "01.01.2001", "01.01.2001");
    table->insertElement(&card2);
    // insert 10 elements for resize
    for (int i = 2; i < 12; i++) {
        LibraryCard card = LibraryCard(i, i, "01.01.2001", "01.01.2001");
        table->insertElement(&card);
    }
    // delete element
    table->deleteElement(1);
    // find element
    HashTableRow *row = table->getElement(2);
    if (row)
        cout << "Найден элемент с ключом 2" << endl;
    else
        cout << "Элемент с ключом 2 не найден" << endl;
}

void testBinWork() {
    // create bin file from txt file
    txtToBin("test.txt", "test.dat");
    printBinFile("test.dat");
    // add new card to bin file
    LibraryCard card = LibraryCard(1, 1, "01.01.2001", "01.01.2001");
    addCard("test.dat", card);
    printBinFile("test.dat");
    // delete card from bin file by key
    deleteCardByKey("test.dat", 1);
    printBinFile("test.dat");
}