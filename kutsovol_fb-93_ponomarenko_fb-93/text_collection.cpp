#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

struct Collection {
	string name;
	vector<string> documents;
	vector<string> nameOfDocuments;
	string setName(string nameOfCollection);
	void insertDocument(string txt);
	void id_document(char name[40]);
	void readDocuments();
	void read_id_document();
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
				//cout << txt << endl;
				collection.insertDocument(txt);
				collection.id_document(str1);
				collection.read_id_document();

				break;

			default:
				collection.readDocuments();
				break;
		}
	}
	
	
}
