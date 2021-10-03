#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

struct Collection {
	string name;
	vector<string> documents;
	vector<string> nameOfDocuments;
	string setName(string nameOfCollection);
	vector<string> words1;
	vector<string> words2;
	vector<string> words3;
	int rows = 3;
	int columns;
	string** documents_collection = new string *[rows];
	//vector<vector<string>> wordsM;
	void insertDocument(string txt);
	void id_document(char name[40]);
	void readDocuments();
	void read_id_document();
	void makeWords();
	void readWords();
};
string Collection::setName(string nameOfCollection) {
	name = nameOfCollection;
	return name;
}
void Collection::insertDocument(string txt) {
	documents.push_back(txt);
}
void Collection::id_document(char name[40]) {
	string str(name);
	nameOfDocuments.push_back(str);
}
void Collection::readDocuments() {
	for (int i = 0; i < documents.size(); i++) {
		cout << nameOfDocuments[i] << ": " << endl;
		cout << documents[i];
		cout << endl;
		cout << "NEXT:" << endl;
	}
}
void Collection::read_id_document() {
	cout << "Document " << nameOfDocuments[nameOfDocuments.size() - 1] << " has been added to " << name << " collection!" << endl;
}
void Collection::makeWords() {
	//vector<vector<string>> welcome(2, vector<string>(3));
	/*vector<vector<string>> welcome;
	welcome.resize(2, vector<string>(3));
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			cin >> welcome[i][j];
		}
	}

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			cout << welcome[i][j] << endl;
		}
	}*/

	
	int text_count = documents.size();
	string newWord;
	int start = 0;
	int space_place = 0;
	int countSymbols = 0;
	int count = 0;

	while (space_place <= documents[0].length() - 1) {
		space_place = documents[0].find(" ", start);
		countSymbols = space_place - count;
		newWord.assign(documents[0], start, countSymbols);
		//вектор векторів із різними довжинами??
		words1.push_back(newWord);
		start = space_place;
		start++;
		count = count + countSymbols + 1;
	}
	start = 0;
	space_place = 0;
	countSymbols = 0;
	count = 0;
	while (space_place <= documents[1].length() - 1) {
		space_place = documents[1].find(" ", start);
		countSymbols = space_place - count;
		newWord.assign(documents[1], start, countSymbols);
		//вектор векторів із різними довжинами??
		words2.push_back(newWord);
		start = space_place;
		start++;
		count = count + countSymbols + 1;
	}
	start = 0;
	space_place = 0;
	countSymbols = 0;
	count = 0;
	while (space_place <= documents[2].length() - 1) {
		space_place = documents[2].find(" ", start);
		countSymbols = space_place - count;
		newWord.assign(documents[2], start, countSymbols);
		//вектор векторів із різними довжинами??
		words3.push_back(newWord);
		start = space_place;
		start++;
		count = count + countSymbols + 1;
	}
	if (words1.size() >= words2.size() and words1.size() >= words3.size()) {
		columns = words1.size();
	}
	else if (words2.size() >= words1.size() and words2.size() >= words3.size()) {
		columns = words2.size();
	}
	else if (words3.size() >= words1.size() and words3.size() >= words2.size()) {
		columns = words3.size();
	}
	for (int i = 0; i < rows; i++) {
		documents_collection[i] = new string[columns];
	}
	for (int j = 0; j < columns; j++) {
		if (j <= (words1.size()-1)) {
			documents_collection[0][j] = words1[j];
		}
		else{
			documents_collection[0][j] = "";
		}
		if (j <= (words2.size() - 1)) {
			documents_collection[1][j] = words2[j];
		}
		else {
			documents_collection[1][j] = "";
		}
		if (j <= (words3.size() - 1)) {
			documents_collection[2][j] = words3[j];
		}
		else {
			documents_collection[2][j] = "";
		}
	}

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			cout << documents_collection[i][j] << "\t";
		}
		cout << '\n';
	}


	/*for (int i = 0; i < text_count; i++) {
		while (space_place <= documents[i].length() - 1) {
			space_place = documents[i].find(" ", start);
			countSymbols = space_place - count;
			newWord.assign(documents[i], start, countSymbols);
			//вектор векторів із різними довжинами??
			words1.push_back(newWord);
			start = space_place;
			start++;
			count = count + countSymbols + 1;
		}
		//перейти на інший вектор
	}*/
	
}
void Collection::readWords() {
	/*int text_count = documents.size();
	for (int i = 0; i < text_count; i++) {
		for (int j = 0; j < count_vect; j++) {
			cout << wordsM[i][j] << endl;
		}
	}*/

	for (int i = 0; i < words1.size(); i++) {
		cout << words1[i] << endl;
	}
	cout << endl;
	for (int i = 0; i < words2.size(); i++) {
		cout << words2[i] << endl;
	}
	cout << endl;
	for (int i = 0; i < words3.size(); i++) {
		cout << words3[i] << endl;
	}
	cout << endl;
}

//читання з файлу
string inputf(ifstream& f, char str[40]) {
	string a;
	f.open(str);
	if (f.fail()) {
		cout << "\n Fail to open the file";
		exit(1);
	}
	string extra = "";
	while (getline(f, a)) {
		extra += a;
	}
	f >> extra;
	f.close();
	return extra;
}

int main() {
	setlocale(LC_ALL, "Russian");
	string txt;
	ifstream f;
	ofstream f1;
	char str1[40], str2[40];

	int exit = 1;
	int choice;
	//реалізувати вектор структур
	//vector<Collection> collections;
	string nameOfCollection;
	//int counter = 0;
	cout << "...Creation of the collection..." << endl;
	cout << "Enter the name of your collection: ";
	cin >> nameOfCollection;
	//collections.push_back(nameOfCollection);
	Collection collection;
	//cout << "Collection " << collection.setName(namesOfCollection[counter]) << " has been created!" << endl;
	cout << "Collection " << collection.setName(nameOfCollection) << " has been created!" << endl;
	while (exit != 0) {
		cout << "1.Insert to collection" << endl;
		cout << "2.Search in collection" << endl;
		cout << "Enter your choice: ";
		cin >> choice;
		switch (choice) {
		case 1:
			//зробити фільтрацію текста(прибрати абзаци, символи)
			cout << "There is a list of all .txt files we have. Choose one you want to insert to your collection:" << endl;
			cout << "Name1" << endl;
			cout << "Name2" << endl;
			cout << "Name3" << endl;
			cout << "Enter the full name (with .txt): ";
			cin >> str1;
			txt = inputf(f, str1);
			cout << endl;
			cout << txt << endl;
			collection.insertDocument(txt);
			collection.id_document(str1);
			collection.read_id_document();
			break;
		case 2:
			collection.makeWords();
			break;

		default:
			collection.readDocuments();
			cout << endl;
			collection.readWords();
			break;
		}
	}
}
