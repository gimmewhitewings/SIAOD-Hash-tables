#pragma once

#include <fstream>
#include <random>
#include <string>
#include <istream>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

struct LibraryCard {
    int inventoryNumber;
    int bookNumber;
    char issueDate[11]{};
    char returnDate[11]{};

    LibraryCard(int inventoryNumber, int bookNumber, const char *issueDate, const char *returnDate) {
        this->inventoryNumber = inventoryNumber;
        this->bookNumber = bookNumber;
        strcpy_s(this->issueDate, issueDate);
        strcpy_s(this->returnDate, returnDate);
    }

    LibraryCard() {
        this->inventoryNumber = 0;
        this->bookNumber = 0;
        this->issueDate[10] = '\0';
        this->returnDate[10] = '\0';
    }
};

void txtToBin(const string &txtFileName, const string &binFileName) {
    ifstream txtFile(txtFileName, ios::in);
    ofstream binFile(binFileName, ios::binary);
    while (!txtFile.eof()) {
        LibraryCard newCard{};
        string issueDate;
        string returnDate;

        txtFile >> newCard.inventoryNumber;
        txtFile >> newCard.bookNumber;
        txtFile.get();
        getline(txtFile, issueDate);
        getline(txtFile, returnDate);
        strcpy_s(newCard.issueDate, issueDate.c_str());
        strcpy_s(newCard.returnDate, returnDate.c_str());
        binFile.write((char *) &newCard, sizeof(LibraryCard));
    }
    txtFile.close();
    binFile.close();
}

void binToTxt(const string &txtFileName, const string &binFileName) {
    ofstream txtFile(txtFileName);
    ifstream binFile(binFileName, ios::binary);
    if (binFile.good()) {
        LibraryCard newCard{};
        binFile.read((char *) &newCard, sizeof(LibraryCard));
        txtFile << newCard.inventoryNumber << '\n'
                << newCard.bookNumber << '\n'
                << newCard.issueDate << '\n'
                << newCard.returnDate;
        while (!binFile.eof()) {
            txtFile << '\n';
            binFile.read((char *) &newCard, sizeof(LibraryCard));
            txtFile << newCard.inventoryNumber << '\n'
                    << newCard.bookNumber << '\n'
                    << newCard.issueDate << '\n'
                    << newCard.returnDate;
        }
    }
    txtFile.close();
    binFile.close();
}

void printBinFile(const string &binFileName) {
    ifstream binFile(binFileName, ios::in | ios::binary);
    LibraryCard card{};
    binFile.seekg(0, ios::beg);
    binFile.read((char *) &card, sizeof(LibraryCard));
    while (!binFile.eof()) {
        cout << card.inventoryNumber << '\n'
             << card.bookNumber << '\n'
             << card.issueDate << '\n'
             << card.returnDate << '\n';
        binFile.read((char *) &card, sizeof(LibraryCard));
    }
}


LibraryCard *getCardPtrByNumber(const string &binFileName, int number) {
    ifstream binFile(binFileName, ios::in | ios::binary);
    LibraryCard *card = new LibraryCard;
    binFile.seekg(sizeof(LibraryCard) * (number - 1), ios::beg);
    if (binFile.read((char *) card,
                     sizeof(LibraryCard))) // Check if there is a record with such a sequential number in the file
    {
        binFile.close();
        return card;
    }
    binFile.close();
    return nullptr;
}

LibraryCard *getCardPtrByKey(const string &binFileName, int inventoryNumber) {
    ifstream binFile(binFileName, ios::in | ios::binary);
    LibraryCard *card = new LibraryCard;
    binFile.seekg(0, ios::beg);
    binFile.read((char *) card, sizeof(LibraryCard));
    while (!binFile.eof()) {
        if (card->inventoryNumber == inventoryNumber) {
            binFile.close();
            return card;
        }
        binFile.read((char *) card, sizeof(LibraryCard));
    }
    binFile.close();
    return nullptr;
}

bool deleteCardByKey(const string &binFileName, int key) {
    LibraryCard lastCard{};
    LibraryCard currentCard{};
    fstream binFile(binFileName, ios::in | ios::out | ios::binary);
    binFile.seekg(-(int) sizeof(LibraryCard), ios::end);
    binFile.read((char *) &lastCard, sizeof(LibraryCard));
    binFile.seekg(0, ios::beg);
    while (binFile.good()) {
        binFile.read((char *) &currentCard, sizeof(LibraryCard));
        if (currentCard.inventoryNumber == key)
        {
            binFile.seekg(-(int) (sizeof(LibraryCard)), ios::cur);
            binFile.write(reinterpret_cast<const char *>(&lastCard),
                          sizeof(LibraryCard));
            binFile.close();
            return true;
        }
    }
    binFile.close();
    return false;
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

vector<int> generateVector(int size) {
    vector<int> vec;
    for (int i = 0; i < size; i++) {
        vec.push_back(i + 1);
    }
    shuffle(vec.begin(), vec.end(), std::mt19937(std::random_device()()));
    return vec;
}

LibraryCard generateCard(int inventoryNumber, int bookNumber) {
    LibraryCard card;
    card.inventoryNumber = inventoryNumber;
    card.bookNumber = bookNumber;
    strcpy_s(card.issueDate, generateDate().c_str());
    strcpy_s(card.returnDate, generateDate().c_str());
    return card;
}

void generateBinFile(const string &binFileName, int count) {
    ofstream binFile(binFileName, ios::binary);
    vector<int> vec = generateVector(count);
    for (int i = 0; i < count; i++) {
        LibraryCard card = generateCard(i + 1, vec.at(i));
        binFile.write((char *) &card, sizeof(LibraryCard));
    }
    binFile.close();
}

void addCard(const string &binFileName, LibraryCard card) {
    ofstream binFile(binFileName, ios::binary | ios::app);
    binFile.write((char *) &card, sizeof(LibraryCard));
    binFile.close();
}
