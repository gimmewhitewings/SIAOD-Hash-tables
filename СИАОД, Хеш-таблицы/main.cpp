#include <iostream>
#include <locale>
#include "HashTable.h"
#include "binWork.h"

using namespace std;


int main() {
	setlocale(LC_ALL, "ru");
	HashTable* table = new HashTable();
	generateBinFile("test.dat", 50);
	printBinFile("test.dat");
	libraryCard* newCard;
	int cardNumber = 1;
	// Заполнение таблицы
	
	while (newCard = getCard("test.dat", cardNumber)) 
	{
		table->insertElement(newCard);
		cardNumber++;
	}
	table->printTable();
	return 0;
}