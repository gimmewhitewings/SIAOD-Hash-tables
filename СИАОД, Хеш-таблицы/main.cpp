#include <iostream>
#include <locale>
#include "HashTable.h"
#include "binWork.h"
#include "HashBinary.h"

using namespace std;

void testHashTable();

int main() {
    setlocale(LC_ALL, "ru");
    /*HashTable* table = new HashTable();*/
    generateBinFile("test.dat", 20);
    /*fillHashTable(table, "test.dat");
    table->printTable();*/
    testHashTable();
    return 0;
}

// Hash table testing
void testHashTable() {
    HashTable *table = new HashTable(13);
    libraryCard *newCard;
    int cardNumber = 1;
    // ���������� �������
    while ((newCard = getCardPtrByNumber("test.dat", cardNumber))) {
        table->insertElement(newCard);
        cardNumber++;
    }
    table->printTable();
    // ����� ��������
    HashTableRow *foundElement = table->getElement(1);
    if (foundElement) {
        cout << "������� ������: " << foundElement->keyInventoryNumber << " " << foundElement->number << " "
             << foundElement->issueDate << " " << foundElement->returnDate << endl;
    } else {
        cout << "������� �� ������" << endl;
    }
    // �������� ��������
    table->deleteElement(1);
    table->printTable();
    // ����� ��������
    foundElement = table->getElement(1);
    if (foundElement) {
        cout << "������� ������: " << foundElement->keyInventoryNumber << " " << foundElement->number << " "
             << foundElement->issueDate << " " << foundElement->returnDate << endl;
    } else {
        cout << "������� �� ������" << endl;
    }
    // ������� �������� � ���������� ��������

}

