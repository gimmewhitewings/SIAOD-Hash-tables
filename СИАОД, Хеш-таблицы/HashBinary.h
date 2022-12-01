#pragma once

#include "binWork.h"
#include "HashTable.h"

using namespace std;

void insertFromFile(HashTable *table, const char *fileName, int key) {
    LibraryCard *newCard = getCardPtrByNumber(fileName, key); // get card from binary file
    if (newCard)
        table->insertElement(newCard); // insert card to hash table
}

void deleteFromFileAndTable(HashTable *table, const char *fileName, int key) {
    LibraryCard *newCard = getCardPtrByKey(fileName, key); // get card from binary file
    if (newCard) {
        table->deleteElement(key); // delete card from hash table
        deleteCardByKey(fileName, key); // delete card from binary file
    }
}

LibraryCard *findInFileWithHashTable(HashTable *table, const char *fileName, int key) {
    LibraryCard *card = getCardPtrByKey(fileName, key); // get card from binary file
    if (card) {
        int number = table->getElement(key)->number; // get card from hash table
        return getCardPtrByNumber(fileName, number);
    }
    return nullptr;
}