#pragma once
#include <fstream>
#include <string>
#include <istream>
#include <iostream>

using namespace std;

struct libraryCard
{
	int number;
	int inventoryNumber;
	char issueDate[11];
	char returnDate[11];
};

void txtToBin(string txtFileName, string binFileName) {
	ifstream txtFile(txtFileName);
	ofstream binFile(binFileName);
	while (!txtFile.eof())
	{
		libraryCard newCard;
		int number;
		int inventoryNumber;
		string issueDate;
		string returnDate;

		txtFile >> newCard.number;
		txtFile >> newCard.inventoryNumber;
		txtFile.get();
		getline(txtFile, issueDate);
		getline(txtFile, returnDate);
		strcpy_s(newCard.issueDate, issueDate.c_str());
		strcpy_s(newCard.returnDate, returnDate.c_str());
		binFile.write((char*)& newCard, sizeof(libraryCard));
	}
	txtFile.close();
	binFile.close();
}

void binToTxt(string txtFileName, string binFileName) {
	ofstream txtFile(txtFileName);
	ifstream binFile(binFileName);
	if (binFile.good())
	{
		libraryCard newCard;
		binFile.read((char*)&newCard, sizeof(libraryCard));
		txtFile << newCard.number << '\n'
			<< newCard.inventoryNumber << '\n'
			<< newCard.issueDate << '\n'
			<< newCard.returnDate;
		while (!binFile.eof())
		{
			txtFile << '\n';
			binFile.read((char*)&newCard, sizeof(libraryCard));
			txtFile << newCard.number << '\n'
				<< newCard.inventoryNumber << '\n'
				<< newCard.issueDate << '\n'
				<< newCard.returnDate;
		}
	}
	txtFile.close();
	binFile.close();
}

void printBinFile(string binFileName) {
	ifstream binFile(binFileName, ios::in | ios::binary);
	libraryCard card;
	binFile.seekg(0, ios::beg);
	binFile.read((char*)&card, sizeof(libraryCard));
	while (!binFile.eof())
	{
		cout << card.number << '\n'
			<< card.inventoryNumber << '\n'
			<< card.issueDate << '\n'
			<< card.returnDate << '\n';
		binFile.read((char*)&card, sizeof(libraryCard));
	}
}

string getCardByNumber(string binFileName, int number) 
{
	ifstream binFile(binFileName);
	libraryCard card;
	string result = "";
	binFile.seekg(sizeof(card) * (number - 1), ios::beg);
	if (binFile.read((char*)&card, sizeof(libraryCard))) // Проверка на существование записи с таким порядковым номером в файле
	{
		result = to_string(card.number) + '\n'
			+ to_string(card.inventoryNumber) + '\n'
			+ card.issueDate + '\n'
			+ card.returnDate;
		binFile.close();
		return result;
	}
	binFile.close();
	return "Запись с таким номером не найдена";
}

 // get card ptr by number
libraryCard* getCardPtrByNumber(string binFileName, int number)
{
	ifstream binFile(binFileName);
	libraryCard* card = new libraryCard;
	binFile.seekg(sizeof(libraryCard) * (number - 1), ios::beg);
	if (binFile.read((char*)card, sizeof(libraryCard))) // Проверка на существование записи с таким порядковым номером в файле
	{
		binFile.close();
		return card;
	}
	binFile.close();
	return nullptr;
}


//libraryCard* getCard(string binFileName, int number)
//{
//	ifstream binFile(binFileName);
//	libraryCard card;
//	binFile.clear();
//	binFile.seekg(sizeof(card) * (number - 1), ios::beg);
//	if (binFile.read((char*)&card, sizeof(libraryCard)))
//	{
//		binFile.close();
//		return &card;
//	}
//	binFile.close();
//	return nullptr;
//}

libraryCard* getCardByKeys(string binFileName, int number, int inventoryNumber) 
{
	ifstream binFile(binFileName);
	libraryCard card;
	while (binFile.good())
	{
		binFile.read((char*)&card, sizeof(libraryCard));
		if (card.number == number && card.inventoryNumber == inventoryNumber)
		{
			binFile.close();
			return &card;
		}
	}
	binFile.close();
	return nullptr;
}

bool deleteCardByKey(string binFileName, int key) {
	libraryCard lastCard;
	libraryCard currentCard;
	fstream binFile(binFileName, ios::in | ios::out | ios::binary);
	binFile.seekg(-(int)sizeof(libraryCard), ios::end); // Перемещаемся на позицию перед последней записью
	binFile.read((char*)&lastCard, sizeof(libraryCard)); // Считываем значение последней карточки
	binFile.seekg(0, ios::beg); // Перемещаемся в начало файла
	while (binFile.good())
	{
		// Считываем по очереди записи в файле
		binFile.read((char*)&currentCard, sizeof(libraryCard));
		if (currentCard.number == key) // Если нашли подходящую по ключу запись
		{
			binFile.seekg(-(int)(sizeof(libraryCard)), ios::cur); // Возвращаемся на одну позицию назад
			binFile.write(reinterpret_cast<const char*>(&lastCard), sizeof(libraryCard)); // Записываем значение последеней записи с заменой текущих данных
			binFile.close();
			return true;
		}
	}
	// Если ничего не нашлось, закрываем файл и возвращаем отрицательный результат выполнения функции
	binFile.close();
	return false;
}

void expiredBooks(string originFileName, string targetFileName, string currentDate) {
	ifstream originFile(originFileName);
	ofstream targetFile(targetFileName);
	libraryCard card;
	originFile.read((char*)&card, sizeof(libraryCard));
	while (originFile.good())
	{
		if (card.returnDate < currentDate)
		{
			targetFile.write((char*)&card, sizeof(libraryCard));
		}
		originFile.read((char*)&card, sizeof(libraryCard));
	}
	originFile.close();
	targetFile.close();
}

void deleteReturnedBooks(string binFileName, string currentDate) {
	ifstream binFile(binFileName, ios::binary);
	libraryCard card;
	ofstream tempBinFile("tempBin.dat", ios::binary);
	binFile.read((char*)&card, sizeof(libraryCard));
	while (binFile.good())
	{
		if (card.returnDate >= currentDate)
		{
			tempBinFile.write((char*)&card, sizeof(libraryCard));
		}
		binFile.read((char*)&card, sizeof(libraryCard));
	}
	binFile.close();
	remove(binFileName.c_str());
	tempBinFile.close();
	rename("tempBin.dat", binFileName.c_str());
}

// generate date yyyy.mm.dd
string generateDate() {
	string date;
	int year = rand() % 10 + 2010;
	int month = rand() % 12 + 1;
	int day = rand() % 28 + 1;
	string monthStr = to_string(month);
	string dayStr = to_string(day);
	if (month < 10) monthStr = "0" + monthStr;
	if (day < 10) dayStr = "0" + dayStr;
	date = to_string(year) + "." + monthStr + "." + dayStr;
	return date;
}

libraryCard generateCard(int number) {
	libraryCard card;
	card.number = number;
	card.inventoryNumber = rand() % 1000 + 1;
	strcpy_s(card.issueDate, generateDate().c_str());
	strcpy_s(card.returnDate, generateDate().c_str());
	return card;
}

void generateBinFile(string binFileName, int count) {
	ofstream binFile(binFileName, ios::binary);
	for (int i = 0; i < count; i++)
	{
		libraryCard card = generateCard(i + 1);
		binFile.write((char*)&card, sizeof(libraryCard));
	}
	binFile.close();
}
