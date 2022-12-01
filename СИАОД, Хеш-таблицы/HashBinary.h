#pragma once

#include "binWork.h"
#include "HashTable.h"

using namespace std;

void insertFromFile(HashTable *table, const char *fileName, int key) {
    LibraryCard *newCard = getCardPtrByKey(fileName, key);
    if (newCard)
        table->insertElement(newCard);
}

void deleteFromFileAndTable(HashTable *table, const char *fileName, int key) {
    LibraryCard *newCard = getCardPtrByKey(fileName, key);
    if (newCard) {
        table->deleteElement(key);
        deleteCardByKey(fileName, key);
    }
}

LibraryCard *findInFileWithHashTable(HashTable *table, const char *fileName, int key) {
    LibraryCard *card = getCardPtrByKey(fileName, key);
    if (card) {
        int number = table->getElement(key)->number;
        return getCardPtrByNumber(fileName, number);
    }
    return nullptr;
}