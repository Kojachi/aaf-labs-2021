#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <list>
#include <cctype>
#include <regex>
#include <map>

using namespace std;

enum Tokens {
	TOKEN_ID, //0
	TOKEN_CREATE, //1
	TOKEN_INSERT, //2
	//TOKEN_DELETE,
	TOKEN_SEARCH, //3
	TOKEN_WHERE, //4
	TOKEN_INDEX, //5
	TOKEN_SEMI, //6
	TOKEN_STRING, //7
	TOKEN_EXIT //8
} type;

struct Lexer {
	string symbol;
	unsigned int position;
	string contents;
	Lexer(string contents_) {
		contents = contents_;
		position = 0;
		symbol = contents[position];
	}
	void newWord(string contents_);
	void deleteAllWords();
	void deleteWord(int place_to_del);
	void lexer_advance();
	void skip_space();
	string lexer_get_current_string();
};

struct Token {
	int type;
	string value;
	int position;
	Token(int type_, string value_, int pos_) {
		type = type_;
		value = value_;
		position = pos_;
	}
	Token(){}
	void init_token(Tokens type_, string value_, int pos_);
	void lexer_advance_with_token(Lexer& lexer);
	void lexer_collect_string(Lexer& lexer);
	string lexer_collect_id(Lexer& lexer);
	bool lexer_get_next_token(Lexer& lexer);
};

string token_types_array[] = {
		{"[cC][rR][eE][aA][tT][eE]"},
		{"[iI][nN][sS][eE][rR][tT]"},
		//{"[dD][eE][lL][eE][tT][eE]([ \t\n\r]{0,}[fF][rR][oO][mM])?"},
		{"[sS][eE][aA][rR][cC][hH]"},
		{"[eE][xX][iI][tT]"},
		{"[pP][rR][iI][nN][tT][_][iI][nN][dD][eE][xX]"},
		{"[wW][hH][eE][rR][eE]"}
};
int token_types_size = sizeof(token_types_array) / sizeof(token_types_array[0]);
int pos;
multimap<string, int> mapOfWords;
vector<multimap<string, int>> vectorOfMaps;
vector<string> nameOfMaps;

void Token::init_token(Tokens type_, string value_, int pos_) {
	type = type_;
	value = value_;
	position = pos_;
}

void Lexer::newWord(string contents_) {
	contents = contents + contents_;
	//cout << "content: " << contents << endl;
	//cout << "position: " << position << endl;
	symbol = contents[position];
	//cout << "symbol: " << symbol << endl;
}
void Lexer::deleteAllWords() {
	contents = "";
	position = 0;
	symbol = contents[position];
}
void Lexer::deleteWord(int place_to_del) {
	string newstr = "";
	for (int i = 0; i < place_to_del; i++) {
		newstr = newstr + contents[i];
	}
	contents = newstr;
	//cout << "contents: " << contents << endl;
	position = place_to_del;
	//cout << "position: " << position << endl;
	//cout << "symbol: " << contents[position - 1] << endl;
}
void Lexer::lexer_advance() {
	if (position < contents.length()) {
		position += 1;
		symbol = contents[position];
	}
}
void Lexer::skip_space() {
	while (symbol == " " or symbol == "\n") {
		lexer_advance();
	}
}
string Lexer::lexer_get_current_string() {
	string str;
	str = symbol;
	//cout << "str: " << str << endl;
	return str;
}

void Token::lexer_advance_with_token(Lexer& lexer) {
	lexer.lexer_advance();
}
void Token::lexer_collect_string(Lexer& lexer) {
	lexer.lexer_advance();
	int pos;
	pos = lexer.position;
	string value = "";
	while (lexer.symbol != "\"") {
		string s = lexer.lexer_get_current_string();
		value = value + s;
		lexer.lexer_advance();
	}
	lexer.lexer_advance();
	init_token(TOKEN_STRING, value, pos);
}
string Token::lexer_collect_id(Lexer &lexer) {
	string value = "";
	int pos;
	pos = lexer.position;
	while ((isalnum(lexer.symbol[0], locale())) or (lexer.symbol == "_")) {
		string s = lexer.lexer_get_current_string();
		value = value + s;
		//cout << "value: " << value << endl;
		lexer.lexer_advance();
	}
	smatch matches;
	for (int i = 0; i < token_types_size; i++) {
		regex regular("^" + token_types_array[i]);
		if (regex_search(value, matches, regular)) {
			if (token_types_array[i] == "[cC][rR][eE][aA][tT][eE]") {
				//if: CREATE + !=TOKEN_ID => ERROR
				init_token(TOKEN_CREATE, value, pos);
				lexer_advance_with_token(lexer);
				return "notId";
			}
			if (token_types_array[i] == "[iI][nN][sS][eE][rR][tT]") {
				//if: INSERT + !=TOKEN_ID + !=TOKEN_STRING => ERROR
				init_token(TOKEN_INSERT, value, pos);
				lexer_advance_with_token(lexer);
				return "notId";
			}
			////if: SEARCH + !=TOKEN_ID or SEARCH + !=TOKEN_ID + !=WHERE + !=TOKEN_STRING => ERROR
			if (token_types_array[i] == "[sS][eE][aA][rR][cC][hH]") {
				init_token(TOKEN_SEARCH, value, pos);
				lexer_advance_with_token(lexer);
				return "notId";
			}
			if (token_types_array[i] == "[wW][hH][eE][rR][eE]") {
				init_token(TOKEN_WHERE, value, pos);
				lexer_advance_with_token(lexer);
				return "notId";
			}
			if (token_types_array[i] == "[eE][xX][iI][tT]") {
				init_token(TOKEN_EXIT, value, pos);
				lexer_advance_with_token(lexer);
				return "notId";
			}
			if (token_types_array[i] == "[pP][rR][iI][nN][tT][_][iI][nN][dD][eE][xX]") {
				init_token(TOKEN_INDEX, value, pos);
				lexer_advance_with_token(lexer);
				return "notId";
			}
		}
	}
	init_token(TOKEN_ID, value, pos);
	return "id";
}
bool Token::lexer_get_next_token(Lexer& lexer) {
	if (lexer.position < (lexer.contents).length()) {
		pos = lexer.position;
	}
	while (lexer.position < (lexer.contents).length()) {
		if (lexer.symbol == " " or lexer.symbol == "\n") {
			lexer.skip_space();
		}
		//if ''
		if (lexer.symbol == "\"") {
			lexer_collect_string(lexer);
			return true;
		}
		if (isalnum(lexer.symbol[0], locale())) {
			lexer_collect_id(lexer);
			return true;
		}
		switch (lexer.symbol[0]) {
		case ';':
			init_token(TOKEN_SEMI, lexer.lexer_get_current_string(), pos);
			lexer_advance_with_token(lexer);
			return true;
			break;
			/*case '(':
				return lexer_advance_with_token(lexer, init_token(TOKEN_LPAREN, lexer_get_current_string(lexer), pos));
				break;
			case ')':
				return lexer_advance_with_token(lexer, init_token(TOKEN_RPAREN, lexer_get_current_string(lexer), pos));
				break;*/
		}
	}
	return false;
}

struct InvertedIndex {
	string name;
	vector<string> word;
	vector<int> word_index;
	vector<int> text_index;
	string word_check;
	vector<int> search_1;
	void print_index(vector<string> text_name);
	void search_keyword(string keyword, vector<string> text_name, vector<string> documents);
	InvertedIndex(string name_) {
		name = name_;
	}
};

void InvertedIndex::print_index(vector<string> text_name) {
	int number_of_table;
	//cout << "Index_name: " << name << endl;
	for (int i = 0; i < nameOfMaps.size(); i++) {
		if (name == nameOfMaps[i]) {
			number_of_table = i;
		}
	}
	//cout << "Number of table: " << number_of_table << endl;
	for (auto it = vectorOfMaps[number_of_table].begin(); it != vectorOfMaps[number_of_table].end(); ++it) {
		if (word_check != it->first) {
			//cout << it->first << " : " << it->second << endl;
			cout << endl;
			cout << it->first << ":" << endl;
		}
		for (int i = 0; i < word.size(); i++) {
			if (it->first == word[i] and word_check != it->first) {
				cout << text_name[text_index[i]] << " -> " << word_index[i] << endl;
			}
		}
		word_check = it->first;
	}
}
void InvertedIndex::search_keyword(string keyword, vector<string> text_name, vector<string> documents) {
	cout << "keyword: " << keyword << endl;
	for (int i = 0; i < word.size(); i++) {
		if (keyword == word[i]) {
			//cout << "word: " << word[i] << endl;
			//cout << "word_index: " << word_index[i] << endl;
			//cout << "text_index: " << text_index[i] << endl;
			search_1.push_back(text_index[i]);
		}
	}
	if (search_1.size() == 0) {
		cout << "\"" << keyword << "\" is not found in " << name << endl;
	}
	else {
		for (int i = 0; i < search_1.size() - 1; i++) {
			if (search_1[i] == search_1[i + 1]) {
				search_1.erase(search_1.begin() + i);
				i--;
			}
		}
		for (int i = 0; i < search_1.size(); i++) {
			cout << documents[search_1[i]] << endl;
		}
		search_1.clear();
	}
}

struct Collection {
	string name;
	vector<string> documents;
	vector<string> nameOfDocuments;
	string getName();
	//vector<string> words1;
	//vector<string> words2;
	//vector<string> words3;
	//int rows = 3;
	//int columns;
	//string** documents_collection = new string * [rows];
	//vector<vector<string>> wordsM;
	void insertDocument(string txt);
	void id_document(string name);
	void search_all();
	//void read_id_document();
	void wordsForInvertedIndex(InvertedIndex& index, int txt_place, int counter_);
	//void readWords();
	Collection(string name_) {
		name = name_;
	}
};
string Collection::getName() {
	return name;
}
void Collection::insertDocument(string txt) {
	documents.push_back(txt);
}
void Collection::id_document(string name) {
	nameOfDocuments.push_back(name);
}
void Collection::search_all() {
	for (int i = 0; i < documents.size(); i++) {
		//cout << nameOfDocuments[i] << ": " << endl;
		cout << documents[i];
		cout << endl;
		//cout << "NEXT:" << endl;
	}
}
void Collection::wordsForInvertedIndex(InvertedIndex& index, int txt_place, int counter_) {
	string newWord;
	int start;
	int space_place;
	int countSymbols;
	int count;
	int wordIndex;
	int textIndex;
	int vect_index;

	start = 0;
	space_place = 0;
	countSymbols = 0;
	count = 0;
	wordIndex = 0;
	//cout << "vector: " << vectorOfMaps.size() << endl;
	if ((vectorOfMaps.size()) <= counter_) {
		while (space_place <= documents[txt_place].length() - 1) {
			space_place = documents[txt_place].find(" ", start);
			countSymbols = space_place - count;
			newWord.assign(documents[txt_place], start, countSymbols);
			//cout << "newWord: " << newWord << endl;
			mapOfWords.insert(pair<string, int>(newWord, wordIndex));
			index.word.push_back(newWord);
			index.text_index.push_back(txt_place);
			index.word_index.push_back(wordIndex);
			start = space_place;
			start++;
			count = count + countSymbols + 1;
			wordIndex++;
		}
		vectorOfMaps.push_back(mapOfWords);
		//cout << "Collection_name: " << name << endl;
		nameOfMaps.push_back(name);
		for (auto it = mapOfWords.begin(); it != mapOfWords.end();) {
			it = mapOfWords.erase(it);
		}
	}
	else {
		for (int i = 0; i < nameOfMaps.size(); i++) {
			if (name == nameOfMaps[i]) {
				vect_index = i;
			}
		}
		while (space_place <= documents[txt_place].length() - 1) {
			space_place = documents[txt_place].find(" ", start);
			countSymbols = space_place - count;
			newWord.assign(documents[txt_place], start, countSymbols);
			vectorOfMaps[vect_index].insert(pair<string, int>(newWord, wordIndex));
			index.word.push_back(newWord);
			index.text_index.push_back(txt_place);
			index.word_index.push_back(wordIndex);
			start = space_place;
			start++;
			count = count + countSymbols + 1;
			wordIndex++;
		}
	}
	/*for (auto it = vectorOfMaps[number_of_table].begin(); it != vectorOfMaps[number_of_table].end(); ++it) {
		cout << it->first << " : " << it->second << endl;
	}*/
}

string inputf(ifstream& f, string str) {
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
	string enter;

	vector<string> namesOfCollection;
	vector<Collection> collections;
	string nameOfCollection, enter_command, command;
	int counter = 0;
	cout << "...One hint: firstly, create a table..." << endl;
	cout << "\"CREATE name;\": ";
	vector<string> tokenList;
	string tokenForList;

	string name_id, place_to_del;
	int start, end, countSymbols, number_of_table;
	bool check_table;

	command = "";
	Lexer lexer(command);
	Token token;
	int count_create = 0;
	int count_break = 0;
	vector<InvertedIndex> indexTable;
	//InvertedIndex indexTable;

	while (true) {
		getline(cin, enter_command, '\n');
		//!!!!!!remove extra spaces!!!!!!!!!!!!
		command = command + enter_command + "\n";
		cout << "Listing of all commands: " << command << endl;
		if (enter_command[0] == '\n') {
			enter_command.erase(0, 1);
		}
		if (enter_command[enter_command.length() - 1] != ';') {
			cout << "The ';' must be at the end of every command!" << endl;
		}
		else {
			lexer.newWord(enter_command);
			while ((token.lexer_get_next_token(lexer)) != false) {
				if (token.type != TOKEN_CREATE and token.type != TOKEN_EXIT and token.position == 0) { //для випадку коли перша команда не create (ERROR)
					cout << "ERROR: please, CREATE TABLE first!!!" << endl;
					lexer.deleteAllWords();
					count_break++;
					break;
				}
				//////////////////////
				if (token.type == TOKEN_ID) { //insert NAME "value" or create NAME or print_index NAME or search NAME
					//cout << "TOKEN_ID" << endl;
					tokenForList = to_string(token.type) + " " + token.value + " " + to_string(token.position);
					tokenList.push_back(tokenForList);
					cout << "Token(" << token.type << ", " << token.value << ", " << token.position << ")" << endl;
					for (count_create; count_create < tokenList.size() - 1; count_create++) {
						//cout << "count_create: " << count_create << endl;
						//cout << "tokenList[count_create]: " << tokenList[count_create] << endl;
						if ((((tokenList[count_create])[0] - '0') == TOKEN_CREATE) and (((tokenList[count_create + 1])[0] - '0') == TOKEN_ID)) { //create NAME ... (true/ERROR)
							start = enter_command.find(" ") + 1;
							//cout << "start: " << start << endl;
							if (enter_command.find(" ", start) == -1) { //для випадку create name (true)
								countSymbols = enter_command.find(";", start) - start;
								//cout << "symbols: " << countSymbols << endl;
								name_id.assign(enter_command, start, countSymbols);
								//cout << name_id << endl;
								namesOfCollection.push_back(name_id);
								collections.push_back(namesOfCollection[counter]);
								cout << "Collection " << collections[counter].getName() << " has been created!" << endl;
								indexTable.push_back(namesOfCollection[counter]);
								counter++;
							}
							else { //для випадку create name "value" (ERROR)
								cout << "It's inappropriate name for table" << endl;
								count_break++;
								start = tokenList[count_create].find(" ", 2) + 1;
								countSymbols = tokenList[count_create].length() - start;
								place_to_del.assign(tokenList[count_create], start, countSymbols);
								lexer.deleteWord(stoi(place_to_del));
								tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
								tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
								break;
							}
						}
						if ((((tokenList[count_create])[0] - '0') == TOKEN_INDEX) and (((tokenList[count_create + 1])[0] - '0') == TOKEN_ID)) { //print_index NAME ... (true/ERROR)
							check_table = false;
							start = enter_command.find(" ") + 1;
							countSymbols = enter_command.find(";", start) - start;
							name_id.assign(enter_command, start, countSymbols);
							for (int i = 0; i < counter; i++) {
								if (collections[i].getName() == name_id) { //якщо таблиця з назвою NAME існує (true)
									cout << "Table " << collections[i].getName() << " exists!" << endl;
									check_table = true;
									number_of_table = i;
								}
							}
							if (check_table == true) {
								if (collections[number_of_table].documents.size() == 0) {
									cout << "Table " << collections[number_of_table].getName() << " is empty!" << endl;
								}
								else {
									cout << "Inverted index for " << collections[number_of_table].getName() << ": " << endl;
									indexTable[number_of_table].print_index(collections[number_of_table].nameOfDocuments);
								}
							}
							else { //якщо таблиця з назвою NAME не існує (ERROR)
								cout << "ERROR: There is no table with this name!!" << endl;
								start = tokenList[count_create].find(" ", 2) + 1;
								countSymbols = tokenList[count_create].length() - start;
								place_to_del.assign(tokenList[count_create], start, countSymbols);
								lexer.deleteWord(stoi(place_to_del));
								tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
								tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
								break;
							}
						}
						if ((((tokenList[count_create])[0] - '0') == TOKEN_SEARCH) and (((tokenList[count_create + 1])[0] - '0') == TOKEN_ID)) {
							check_table = false;
							start = enter_command.find(" ") + 1;
							//cout << "start: " << start << endl;
							if (enter_command.find(" ", start) == -1) { //для випадку search NAME
								countSymbols = enter_command.find(";", start) - start;
								name_id.assign(enter_command, start, countSymbols);
								for (int i = 0; i < counter; i++) {
									if (collections[i].getName() == name_id) { //якщо таблиця з назвою NAME існує (true)
										cout << "Table " << collections[i].getName() << " exists!" << endl;
										check_table = true;
										number_of_table = i;
									}
								}
								if (check_table == true) {
									if (collections[number_of_table].documents.size() == 0) {
										cout << "Table " << collections[number_of_table].getName() << " is empty!" << endl;
									}
									else {
										collections[number_of_table].search_all();
									}
								}
								else { //якщо таблиця з назвою NAME не існує (ERROR)
									cout << "ERROR: There is no table with this name!!" << endl;
									start = tokenList[count_create].find(" ", 2) + 1;
									countSymbols = tokenList[count_create].length() - start;
									place_to_del.assign(tokenList[count_create], start, countSymbols);
									lexer.deleteWord(stoi(place_to_del));
									tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
									tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
									break;
								}
							}
							else { //для випадку search NAME ...
								countSymbols = enter_command.find(" ", start) - start;
								name_id.assign(enter_command, start, countSymbols);
								for (int i = 0; i < counter; i++) {
									if (collections[i].getName() == name_id) { //якщо таблиця з назвою NAME існує (true)
										cout << "Table " << collections[i].getName() << " exists!" << endl;
										check_table = true;
										number_of_table = i;
									}
								}
								if (check_table == false) {
									cout << "ERROR: There is no table with this name!!" << endl;
									start = tokenList[count_create].find(" ", 2) + 1;
									countSymbols = tokenList[count_create].length() - start;
									place_to_del.assign(tokenList[count_create], start, countSymbols);
									lexer.deleteWord(stoi(place_to_del));
									tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
									tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
									break;
								}
							}
						}
					}
				}
				else if (token.type == TOKEN_STRING and count_break == 0) { //insert name "VALUE" or search name where "CONDITION"
					//cout << "TOKEN_STRING" << endl;
					tokenForList = to_string(token.type) + " " + token.value + " " + to_string(token.position);
					tokenList.push_back(tokenForList);
					cout << "Token(" << token.type << ", " << token.value << ", " << token.position << ")" << endl;
					for (count_create; count_create < tokenList.size() - 1; count_create++) {
						//cout << "STRINGS: count_create: " << count_create << endl;
						//cout << "STRINGS: tokenList[count_create]: " << tokenList[count_create] << endl;
						if ((((tokenList[count_create])[0] - '0') == TOKEN_CREATE /* != TOKEN_ID*/)) { //для випадку create "name" (ERROR)
							break;
						}
						if ((((tokenList[count_create - 1])[0] - '0') == TOKEN_INSERT) and (((tokenList[count_create])[0] - '0') == TOKEN_ID) and (((tokenList[count_create + 1])[0] - '0') == TOKEN_STRING)) { //insert name "VALUE" (true or ERROR)
							check_table = false;
							start = enter_command.find(" ") + 1;
							countSymbols = enter_command.find(" ", start) - start;
							name_id.assign(enter_command, start, countSymbols);
							for (int i = 0; i < counter; i++) {
								if (collections[i].getName() == name_id) {
									cout << "Table " << collections[i].getName() << " exists!" << endl;
									check_table = true;
									number_of_table = i;
								}
							}
							if (check_table == true) { //якщо таблиця з назвою NAME існує (true)
								start = tokenList[count_create + 1].find(" ") + 1;
								countSymbols = tokenList[count_create + 1].find(" ", start) - start;
								name_id.assign(tokenList[count_create + 1], start, countSymbols);
								txt = inputf(f, name_id);
								//cout << "Txt: " << txt << endl;
								collections[number_of_table].insertDocument(txt);
								cout << "Document has been added to " << collections[number_of_table].getName() << endl;
								collections[number_of_table].id_document(name_id);

								//collections[number_of_table].readDocuments();

								//////new (30.10.21)//////
								//cout << "table #" << number_of_table << endl;
								collections[number_of_table].wordsForInvertedIndex(indexTable[number_of_table], collections[number_of_table].documents.size()-1, counter-1);
							}
							else { //якщо таблиця з назвою NAME не існує (ERROR)
								cout << "ERROR: There is no table with this name!!" << endl;
								start = tokenList[count_create - 1].find(" ", 2) + 1;
								countSymbols = tokenList[count_create - 1].length() - start;
								place_to_del.assign(tokenList[count_create - 1], start, countSymbols);
								lexer.deleteWord(stoi(place_to_del));
								tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
								tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
								tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
								break;
							}
						}
						else if ((((tokenList[count_create - 2])[0] - '0') == TOKEN_SEARCH) and (((tokenList[count_create - 1])[0] - '0') == TOKEN_ID) and (((tokenList[count_create])[0] - '0') == TOKEN_WHERE) and (((tokenList[count_create + 1])[0] - '0') == TOKEN_STRING)) {
							//cout << "search where" << endl;
							start = tokenList[count_create + 1].find(" ") + 1;
							countSymbols = tokenList[count_create + 1].find(" ", start) - start;
							name_id.assign(tokenList[count_create + 1], start, countSymbols);
							//cout << "table #" << number_of_table << endl;
							if (collections[number_of_table].documents.size() == 0) {
								cout << "Table " << collections[number_of_table].getName() << " is empty!" << endl;
							}
							else {
								indexTable[number_of_table].search_keyword(name_id, collections[number_of_table].nameOfDocuments, collections[number_of_table].documents);
							}
						}
					}
				}
				/*else if (token.type == TOKEN_INDEX) {
					cout << "TOKEN_INDEX" << endl;
				}*/
				else if (token.type == TOKEN_WHERE) {
					tokenForList = to_string(token.type) + " " + token.value + " " + to_string(token.position);
					tokenList.push_back(tokenForList);
					cout << "Token(" << token.type << ", " << token.value << ", " << token.position << ")" << endl;
					count_create++;
				}
				else { //для ";", create
					//cout << "TOKEN_..." << endl;
					tokenForList = to_string(token.type) + " " + token.value + " " + to_string(token.position);
					tokenList.push_back(tokenForList);
					cout << "Token(" << token.type << ", " << token.value << ", " << token.position << ")" << endl;
				}
				//////////////////////
				if (token.type == TOKEN_EXIT) { //завершення
					exit(1);
				}
			}
			if (count_break == 0) {
				for (count_create; count_create < tokenList.size() - 1; count_create++) {
					//cout << count_create << endl;
					if ((((tokenList[count_create])[0] - '0') == TOKEN_CREATE) and ((((tokenList[count_create + 1])[0] - '0') == TOKEN_INSERT) or (((tokenList[count_create + 1])[0] - '0') == TOKEN_SEMI))) { //для випадку create INSERT;/create ; (ERROR)
						cout << "It's inappropriate name for table" << endl;
						start = tokenList[count_create].find(" ", 2) + 1;
						countSymbols = tokenList[count_create].length() - start;
						//cout << "start: " << start << endl;
						//cout << "countSymbols: " << countSymbols << endl;
						place_to_del.assign(tokenList[count_create], start, countSymbols);
						//cout << "place: " << place_to_del << endl;
						lexer.deleteWord(stoi(place_to_del));
						tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
						tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
						if (count_create == 0) {
							break;
						}
						else {
							count_create--;
							break;
						}
					}
					else if ((((tokenList[count_create])[0] - '0') == TOKEN_CREATE) and (((tokenList[count_create + 1])[0] - '0') == TOKEN_STRING)) { //для випадку create "name" (ERROR) продовження
						cout << "Don't use the \"\" to name your table!" << endl;
						start = tokenList[count_create].find(" ", 2) + 1;
						countSymbols = tokenList[count_create].length() - start;
						//cout << "start: " << start << endl;
						//cout << "countSymbols: " << countSymbols << endl;
						place_to_del.assign(tokenList[count_create], start, countSymbols);
						//cout << "place: " << place_to_del << endl;
						lexer.deleteWord(stoi(place_to_del));
						tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
						tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
						tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
						if (count_create == 0) {
							break;
						}
						else {
							count_create--;
							break;
						}
					}
				}
			}
			count_break = 0;
		}
	}
}