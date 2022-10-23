#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include "binWork.h"

using namespace std;


struct HashTableRow
{
	int keyInventoryNumber;
	int number;
	char issueDate[11];
	char returnDate[11];

	HashTableRow(const int keyInventoryNumber,
		const int number,
		const char* issueDate,
		const char* returnDate) 
	{
		this->keyInventoryNumber = keyInventoryNumber;
		this->number = number;
		strcpy_s(this->issueDate, issueDate);
		strcpy_s(this->returnDate, returnDate);
	}

	HashTableRow()
	{
		this->keyInventoryNumber = 0;
		this->number = 0;
		this->issueDate[10] = '\0';
		this->returnDate[10] = '\0';
	}
};

struct HashTable 
{
private:
	unsigned size;
	unsigned closedAddresses = 0;
	HashTableRow* table;
	int firstHash(int key, unsigned size);
	int secondHash(int key, unsigned size);
	void resizeTable();

public:
	HashTable(unsigned size);
	void insertElement(libraryCard* libraryCard);
	HashTableRow* getElement(int key);
	void deleteElement(int key);
	void printTable();
	unsigned getSize();
};


inline int HashTable::firstHash(int key, unsigned size)
{
	return ((key * 18 / 9) * 12 % 21) % size;
}

inline int HashTable::secondHash(int key, unsigned size)
{
	return key / size;
}

// rehash table
inline void HashTable::resizeTable()
{
	HashTableRow* oldTable = table;
	unsigned oldSize = size;
	size = size * 2 + 1;
	table = new HashTableRow[size];
	closedAddresses = 0;
	for (unsigned i = 0; i < oldSize; i++)
	{
		if (oldTable[i].keyInventoryNumber != 0)
		{
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

//inline void HashTable::resizeTable()
//{
//	this->closedAddresses = 0;
//	unsigned newSize = size * 2;
//	HashTableRow* newTable = new HashTableRow[size];
//
//	for (size_t i = 0; i < size; i++)
//	{
//		if (table[i].keyInventoryNumber > 0)
//		{
//			HashTableRow currentElement = table[i];
//
//			int key = currentElement.keyInventoryNumber;
//			int index = firstHash(key, newSize);
//
//			while (table[index].keyInventoryNumber != 0)
//			{
//				if (table[index].keyInventoryNumber == -1)
//				{
//					break;
//				}
//				index = (index + secondHash(key, newSize)) % newSize;
//			}
//			newTable[index] = currentElement;
//			closedAddresses++;
//		}
//	}
//
//	delete[] table;
//	table = newTable;
//	size = newSize;
//}

HashTable::HashTable(unsigned size = 13)
{
	this->size = size;
	table = new HashTableRow[size];
}

void HashTable::insertElement(libraryCard* libraryCard)
{
	HashTableRow* currentElement = new HashTableRow(
		libraryCard->inventoryNumber,
		libraryCard->number, 
		libraryCard->issueDate, 
		libraryCard->returnDate
	);

	int key = currentElement->keyInventoryNumber;
	int index = firstHash(key, size);

	while (table[index].keyInventoryNumber != 0)
	{
		if (table[index].keyInventoryNumber == -1)
		{
			break;
		}
		index = (index + secondHash(key, this->size)) % this->size;
	}

	table[index] = *currentElement;
	this->closedAddresses++;

	if (closedAddresses / (double)size > 0.75)
	{
		this->resizeTable();
	}
}

inline HashTableRow* HashTable::getElement(int key)
{
	int index = firstHash(key, size);
	while (table[index].keyInventoryNumber > 0)
	{
		if (table[index].keyInventoryNumber == key)
		{
			return &table[index];
		}
		else
		{
			index = (index + secondHash(key, this->size)) % this->size;
		}
	}
	return nullptr;
}


inline void HashTable::printTable()
{
	int indexFieldSize = to_string(size).length();
	string horizontalDelimeter = "";
	string verticalDelimeter = " | ";
		
	for (int i = 0; i < indexFieldSize + 55; i++)
	{
		horizontalDelimeter += "_";
	}
	
	cout << horizontalDelimeter << endl;
	cout << "|" << setw(indexFieldSize) << "index" << verticalDelimeter
		<< setw(13) << "keyInventoryNumber" << verticalDelimeter
		<< setw(5) << "number" << verticalDelimeter
		<< setw(10) << "issueDate" << verticalDelimeter
		<< setw(10) << "returnDate" << verticalDelimeter << endl;
	cout << horizontalDelimeter << endl;

	for (int i = 0; i < size; i++)
	{
		cout << "| " << setw(indexFieldSize)
			<< i + 1 << verticalDelimeter;
		if (table[i].keyInventoryNumber > 0)
		{
			cout << setw(13) << table[i].keyInventoryNumber << verticalDelimeter
				<< setw(5) << table[i].number << verticalDelimeter
				<< setw(10) << table[i].issueDate << verticalDelimeter
				<< setw(10) << table[i].returnDate;
		}
		else
		{
			cout << setw(13) << " " << verticalDelimeter
				<< setw(5) << " " << verticalDelimeter
				<< setw(10) << " " << verticalDelimeter
				<< setw(10) << " ";
		}
		cout << " |" << '\n' << horizontalDelimeter << '\n';
	}
}

void HashTable::deleteElement(int key)
{
	int index = firstHash(key, this->size);
	while (table[index].keyInventoryNumber != 0)
	{
		if (table[index].keyInventoryNumber == key)
		{
			table[index].keyInventoryNumber = -1;
			return;
		}
		else
		{
			index = (index + secondHash(key, this->size)) % this->size;
		}
	}
}

unsigned HashTable::getSize()
{
	return size;
}