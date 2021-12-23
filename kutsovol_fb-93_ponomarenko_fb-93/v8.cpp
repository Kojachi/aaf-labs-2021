#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <iterator>
#include <fstream>

using namespace std;

enum Tokens {
	TOKEN_ID, //0
	TOKEN_CREATE, //1
	TOKEN_INSERT, //2
	TOKEN_SEARCH, //3
	TOKEN_WHERE, //4
	TOKEN_INDEX, //5
	TOKEN_SEMI, //6
	TOKEN_STRING, //7
	TOKEN_STAR, //8
	TOKEN_NUMBER, //9
	TOKEN_EXIT //10
} type;
string token_types_array[] = {
		{"[cC][rR][eE][aA][tT][eE]"},
		{"[iI][nN][sS][eE][rR][tT]"},
		{"[sS][eE][aA][rR][cC][hH]"},
		{"[eE][xX][iI][tT]"},
		{"[pP][rR][iI][nN][tT][_][iI][nN][dD][eE][xX]"},
		{"[wW][hH][eE][rR][eE]"}
};
int token_types_size = sizeof(token_types_array) / sizeof(token_types_array[0]);

struct Lexer {
	string symbol;
	unsigned int position;
	string contents;
	string newstr, str;
	Lexer(string contents_) {
		contents = contents_;
		position = 0;
		symbol = contents[position];
	}
	void newWord(string contents_);
	void deleteAllWords();
	void deleteWords(int place_to_del);
	void lexer_advance();
	void skip_space();
	string lexer_get_current_string();
};
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
void Lexer::deleteWords(int place_to_del) {
	newstr = "";
	for (int i = 0; i < place_to_del; i++) {
		newstr = newstr + contents[i];
	}
	contents = newstr;
	//cout << "contents: " << contents << endl;
	position = place_to_del;
	//cout << "position: " << position << endl;
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
	str = symbol;
	return str;
}

struct Token {
	int type;
	string value;
	int position;
	string str_value, symbol;
	int pos, pos_global;
	Token(int type_, string value_, int pos_) {
		type = type_;
		value = value_;
		position = pos_;
	}
	Token() {}
	void init_token(Tokens type_, string value_, int pos_);
	void lexer_advance_with_token(Lexer& lexer);
	void lexer_collect_string(Lexer& lexer);
	void lexer_collect_number(Lexer& lexer);
	void lexer_collect_id(Lexer& lexer);
	bool lexer_get_next_token(Lexer& lexer);
};
void Token::init_token(Tokens type_, string value_, int pos_) {
	type = type_;
	value = value_;
	position = pos_;
}
void Token::lexer_advance_with_token(Lexer& lexer) {
	lexer.lexer_advance();
}
void Token::lexer_collect_string(Lexer& lexer) {
	lexer.lexer_advance();
	str_value = "";
	pos = lexer.position;
	while (lexer.symbol != "\"") {
		symbol = lexer.lexer_get_current_string();
		str_value = str_value + symbol;
		lexer.lexer_advance();
	}
	lexer.lexer_advance();
	init_token(TOKEN_STRING, str_value, pos);
}
void Token::lexer_collect_number(Lexer& lexer) {
	lexer.lexer_advance();
	str_value = "";
	pos = lexer.position;
	while (lexer.symbol != ">") {
		symbol = lexer.lexer_get_current_string();
		str_value = str_value + symbol;
		lexer.lexer_advance();
	}
	lexer.lexer_advance();
	init_token(TOKEN_NUMBER, str_value, pos);
}
void Token::lexer_collect_id(Lexer& lexer) {
	bool check_id = true;
	str_value = "";
	pos = lexer.position;
	while ((isalnum(lexer.symbol[0], locale())) or (lexer.symbol == "_")) {
		symbol = lexer.lexer_get_current_string();
		str_value = str_value + symbol;
		lexer.lexer_advance();
	}
	smatch matches;
	for (int i = 0; i < token_types_size; i++) {
		regex regular("^" + token_types_array[i]);
		if (regex_search(str_value, matches, regular)) {
			if (token_types_array[i] == "[cC][rR][eE][aA][tT][eE]") {
				check_id = false;
				init_token(TOKEN_CREATE, str_value, pos);
				if (lexer.symbol[0] == ';') {
					//cout << "l.s ;: " << lexer.symbol[0] << endl;
				}
				else {
					lexer_advance_with_token(lexer);
					//cout << "l.s w/o ;: " << lexer.symbol[0] << endl;
				}
			}
			if (token_types_array[i] == "[iI][nN][sS][eE][rR][tT]") {
				check_id = false;
				init_token(TOKEN_INSERT, str_value, pos);
				if (lexer.symbol[0] == ';') {
					//cout << "l.s ;: " << lexer.symbol[0] << endl;
				}
				else {
					lexer_advance_with_token(lexer);
					//cout << "l.s w/o ;: " << lexer.symbol[0] << endl;
				}
			}
			if (token_types_array[i] == "[sS][eE][aA][rR][cC][hH]") {
				check_id = false;
				init_token(TOKEN_SEARCH, str_value, pos);
				if (lexer.symbol[0] == ';') {
					//cout << "l.s ;: " << lexer.symbol[0] << endl;
				}
				else {
					lexer_advance_with_token(lexer);
					//cout << "l.s w/o ;: " << lexer.symbol[0] << endl;
				}
			}
			if (token_types_array[i] == "[wW][hH][eE][rR][eE]") {
				check_id = false;
				init_token(TOKEN_WHERE, str_value, pos);
				if (lexer.symbol[0] == ';') {
					//cout << "l.s ;: " << lexer.symbol[0] << endl;
				}
				else {
					lexer_advance_with_token(lexer);
					//cout << "l.s w/o ;: " << lexer.symbol[0] << endl;
				}
			}
			if (token_types_array[i] == "[eE][xX][iI][tT]") {
				check_id = false;
				init_token(TOKEN_EXIT, str_value, pos);
				lexer_advance_with_token(lexer);
			}
			if (token_types_array[i] == "[pP][rR][iI][nN][tT][_][iI][nN][dD][eE][xX]") {
				check_id = false;
				init_token(TOKEN_INDEX, str_value, pos);
				if (lexer.symbol[0] == ';') {
					//cout << "l.s ;: " << lexer.symbol[0] << endl;
				}
				else {
					lexer_advance_with_token(lexer);
					//cout << "l.s w/o ;: " << lexer.symbol[0] << endl;
				}
			}
		}
	}
	if (check_id == true) {
		init_token(TOKEN_ID, str_value, pos);
	}
}
bool Token::lexer_get_next_token(Lexer& lexer) {
	if (lexer.position < (lexer.contents).length()) {
		pos_global = lexer.position;
	}
	while (lexer.position < (lexer.contents).length()) {
		if (lexer.symbol == " " or lexer.symbol == "\n") {
			lexer.skip_space();
		}
		if (lexer.symbol == "\"") {
			lexer_collect_string(lexer);
			return true;
		}
		if (lexer.symbol == "<") {
			lexer_collect_number(lexer);
			return true;
		}
		if (isalnum(lexer.symbol[0], locale())) {
			lexer_collect_id(lexer);
			return true;
		}
		switch (lexer.symbol[0]) {
		case ';':
			init_token(TOKEN_SEMI, lexer.lexer_get_current_string(), pos_global);
			lexer_advance_with_token(lexer);
			return true;
			break;
		case '*':
			init_token(TOKEN_STAR, lexer.lexer_get_current_string(), pos_global);
			lexer_advance_with_token(lexer);
			return true;
			break;
		}
	}
	return false;
}

struct InvertedIndex {
	string name;
	vector<string> word_value;
	vector<int> word_index;
	vector<int> text_index;
	string word_check;
	vector<int> search_1, search_2, search_3;
	int number_of_table;
	multimap<string, int> mapOfWords;//мапа зі слів та відповідного індексу в тексті(тому і мультімапа, адже в одному тексті одне й те ж саме слово може зустрічатися декілька разів, а отже і мати декілька індексів)
	multimap<string, int>mapOfTexts;
	void print_index(vector<string> text_name);
	void search_keyword(string keyword, vector<string> text_name, vector<string> documents);
	void search_prefix(string keyword, vector<string> text_name, vector<string> documents);
	void search_two_key(string key1, string key2, int n, vector<string> text_name, vector<string> documents);
	InvertedIndex(string name_) {
		name = name_;
	}
};
void InvertedIndex::print_index(vector<string> text_name) {
	word_check = "";

	for (auto it = mapOfWords.begin(); it != mapOfWords.end(); ++it) {
		if (word_check != it->first) {
			cout << endl;
			cout << it->first << ":" << endl;
		}
		if (word_check != it->first) {
			for (int i = 0; i < word_value.size(); i++) {//рухаємося по всім словам усіх текстів нашої таблиці
				if (it->first == word_value[i]) {
					cout << text_name[text_index[i]] << " -> " << word_index[i] << endl;//виводимо назву текста, в якому дане слово є, та місце слова у цьому тексті
				}
			}
		}
		word_check = it->first;
	}
}
void InvertedIndex::search_keyword(string keyword, vector<string> text_name, vector<string> documents) {
	transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);
	typedef multimap<string, int> ::iterator iter;
	pair<iter, iter> result = mapOfTexts.equal_range(keyword);
	for (iter it = result.first; it != result.second; it++) {
		//cout << "Place: " << it->second << endl;
		search_1.push_back(it->second);
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
	
	/*for (int i = 0; i < word_value.size(); i++) {
		if (keyword == word_value[i]) {
			//cout << "word: " << word_value[i] << endl;
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
	}*/
}
void InvertedIndex::search_prefix(string keyword, vector<string> text_name, vector<string> documents) {
	transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);
	for (auto it = mapOfTexts.begin(); it != mapOfTexts.end(); ++it) {
		if (it->first.find(keyword) == 0) {
			search_2.push_back(it->second);
		}
	}
	if (search_2.size() == 0) {
		cout << "Word that has prefix \"" << keyword << "\" is not found in " << name << endl;
	}
	else {
		for (int i = 0; i < search_2.size() - 1; i++) {
			if (search_2[i] == search_2[i + 1]) {
				search_2.erase(search_2.begin() + i);
				i--;
			}
		}
		for (int i = 0; i < search_2.size(); i++) {
			cout << documents[search_2[i]] << endl;
		}
		search_2.clear();
	}

	/*for (int i = 0; i < word_value.size(); i++) {
		if (word_value[i].find(keyword) == 0) {
			//cout << "word: " << word_value[i] << endl;
			//cout << "word_index: " << word_index[i] << endl;
			//cout << "text_index: " << text_index[i] << endl;
			search_2.push_back(text_index[i]);
		}
	}
	if (search_2.size() == 0) {
		cout << "Word that has prefix \"" << keyword << "\" is not found in " << name << endl;
	}
	else {
		for (int i = 0; i < search_2.size() - 1; i++) {
			if (search_2[i] == search_2[i + 1]) {
				search_2.erase(search_2.begin() + i);
				i--;
			}
		}
		for (int i = 0; i < search_2.size(); i++) {
			cout << documents[search_2[i]] << endl;
		}
		search_2.clear();
	}*/
}
void InvertedIndex::search_two_key(string key1, string key2, int n, vector<string> text_name, vector<string> documents) {
	transform(key1.begin(), key1.end(), key1.begin(), ::tolower);
	transform(key2.begin(), key2.end(), key2.begin(), ::tolower);

	typedef multimap<string, int> ::iterator iterT1, iterT2, iter1, iter2;
	pair<iterT1, iterT1> resultT1 = mapOfTexts.equal_range(key1);
	pair<iterT2, iterT2> resultT2 = mapOfTexts.equal_range(key2);
	pair<iter1, iter1> result1 = mapOfWords.equal_range(key1);
	pair<iter2, iter2> result2 = mapOfWords.equal_range(key2);

	iterT1 itT1 = resultT1.first;
	iterT2 itT2 = resultT2.first;
	for (iter1 it1 = result1.first; it1 != result1.second; it1++) {
		for (iter2 it2 = result2.first; it2 != result2.second; it2++) {
			if (mapOfWords.size() > (it1->second) + n) {
				if ((it1->second) + n == it2->second and itT1->second == itT2->second) {
					//cout << key1 << ": " << it1->second << " Text#" << itT1->second << endl;
					search_3.push_back(itT1->second);
				}
				if ((it2->second) + n == it1->second and itT2->second == itT1->second) {
					//cout << key1 << ": " << it2->second << " Text#" << itT2->second << endl;
					search_3.push_back(itT2->second);
				}
			}
		}
	}
	if (search_3.size() == 0) {
		cout << "Search query \"" << key1 << "\"<" << n << ">\"" << key2 << "\" is not found in " << name << endl;
	}
	else {
		for (int i = 0; i < search_3.size() - 1; i++) {
			if (search_3[i] == search_3[i + 1]) {
				search_3.erase(search_3.begin() + i);
				i--;
			}
		}
		for (int i = 0; i < search_3.size(); i++) {
			cout << documents[search_3[i]] << endl;
		}
		search_3.clear();
	}

	/*for (int i = 0; i < word_value.size(); i++) {
		if (word_value[i] == key1) {
			if (word_value.size() > i + n) {
				if (word_value[i + n] == key2 and text_index[i] == text_index[i + n]) {
					//cout << "word1: " << word_value[i] << endl;
					//cout << "word2: " << word_value[i + n] << endl;
					//cout << "word_index: " << word_index[i] << " and " << word_index[i + n] << endl;
					//cout << "text_index: " << text_index[i] << endl;
					search_3.push_back(text_index[i]);
				}
			}
		}
		else if (word_value[i] == key2) {
			if (word_value.size() > i + n) {
				if (word_value[i + n] == key1 and text_index[i] == text_index[i + n]) {
					//cout << "word1: " << word_value[i] << endl;
					//cout << "word2: " << word_value[i + n] << endl;
					//cout << "word_index: " << word_index[i] << " and " << word_index[i + n] << endl;
					//cout << "text_index: " << text_index[i] << endl;
					search_3.push_back(text_index[i]);
				}
			}
		}
	}
	if (search_3.size() == 0) {
		cout << "Search query \"" << key1 << "\"<" << n << ">\"" << key2 << "\" is not found in " << name << endl;
	}
	else {
		for (int i = 0; i < search_3.size() - 1; i++) {
			if (search_3[i] == search_3[i + 1]) {
				search_3.erase(search_3.begin() + i);
				i--;
			}
		}
		for (int i = 0; i < search_3.size(); i++) {
			cout << documents[search_3[i]] << endl;
		}
		search_3.clear();
	}*/
}

struct Collection {
	string name;
	vector<string> documents;
	vector<string> nameOfDocuments;
	string getName();
	void insertDocument(string txt, string name);
	void search_all();
	void wordsForInvertedIndex(InvertedIndex& index, int txt_place);
	Collection(string name_) {
		name = name_;
	}
};
string Collection::getName() {
	return name;
}
void Collection::insertDocument(string txt, string name) {
	documents.push_back(txt);
	nameOfDocuments.push_back(name);
}
void Collection::search_all() {
	for (int i = 0; i < documents.size(); i++) {
		cout << documents[i] << endl;
		cout << endl;
	}
}
void Collection::wordsForInvertedIndex(InvertedIndex& index, int txt_place) {
	string newWord;
	int start = 0;
	int space_place = 0;
	int countSymbols = 0;
	int wordIndex = 0;
	
	while (space_place < documents[txt_place].length()) {
		if (documents[txt_place].find(" ", start) == -1) {
			space_place = documents[txt_place].length();
			countSymbols = documents[txt_place].length() - start;
			newWord.assign(documents[txt_place], start, countSymbols);
		}
		else {
			space_place = documents[txt_place].find(" ", start) + 1;
			countSymbols = documents[txt_place].find(" ", start) - start;
			newWord.assign(documents[txt_place], start, countSymbols);
		}
		index.mapOfWords.insert(pair<string, int>(newWord, wordIndex));
		index.mapOfTexts.insert(pair<string, int>(newWord, txt_place));
		index.word_value.push_back(newWord);
		index.text_index.push_back(txt_place);
		index.word_index.push_back(wordIndex);
		start = space_place;
		wordIndex++;
	}
}

string inputf(ifstream& f, string str) {
	string a;
	f.open(str);
	if (f.fail()) {
		return "error";
	}
	string extra = "";
	while (getline(f, a)) {
		extra += a;
	}
	f >> extra;
	f.close();
	return extra;
}
string replace_tabsAndSpaces(string enter_command) {
	for (int i = 0; i < enter_command.length(); i++) {
		if (enter_command[i] == '\t') {
			enter_command.replace(i, 1, " "); //замінюємо таб на один пробіл
		}
	}
	for (int i = enter_command.length() - 1; i >= 0; i--) {
		if (i == 1 and enter_command[0] == ' ' and enter_command[i] != ' ') {
			enter_command.erase(0, 1);
			break;
		}
		if (enter_command[i] == ' ' and enter_command[i - 1] == ' ') { //якщо два пробіли подряд, видаляємо один
			enter_command.erase(enter_command.begin() + i);
			if (i == 1 and enter_command[0] == ' ') {
				enter_command.erase(0, 1);
			}
		}
	}
	if (enter_command.length() == 1 and enter_command[enter_command.length() - 1] == ';') {

	}
	else if (enter_command[enter_command.length() - 1] == ';' and enter_command[enter_command.length() - 2] == ' ') { //прибираємо зайвий пробіл в кінці(якщо той мається взагалі)
		enter_command.erase(enter_command.begin() + enter_command.length() - 2);
	}
	if (enter_command[0] == '\n') {
		enter_command.erase(0, 1);
	}
	return enter_command;
}
string make_name(string word) {
	int start;
	int countSymbols;
	string name_id;
	start = word.find(" ") + 1;
	countSymbols = word.find(" ", start) - start;
	name_id.assign(word, start, countSymbols);
	return name_id;
}
string make_name2(string word, vector<string> tokenList) {
	int place;
	int countSymbols;
	string name_id;
	countSymbols = word.find(" ", 0);
	if (countSymbols < 0) {
		countSymbols = word.find(";", 0);
	}
	name_id.assign(word, 0, countSymbols);
	for (int i = 0; i < tokenList.size(); i++) {
		if (tokenList[i].find(name_id) != -1) {
			place = i;
		}
	}
	return tokenList[place];
}
string make_name3(string word, vector<string> tokenList) {
	int place;
	int countSymbols;
	string name_id;
	countSymbols = word.find(" ", 0);
	if (countSymbols < 0) {
		countSymbols = word.find(";", 0);
	}
	name_id.assign(word, 0, countSymbols);
	if (name_id.find("\"") != -1) {
		name_id.erase(0, 1);
		name_id.erase(name_id.size() - 1, 1);
		//cout << name_id << endl;
	}
	if (name_id.find("<") != -1) {
		name_id.erase(0, 1);
		name_id.erase(name_id.size() - 1, 1);
		//cout << name_id << endl;
	}
	for (int i = 0; i < tokenList.size(); i++) {
		if (tokenList[i].find(name_id) != -1) {
			place = i;
		}
	}
	if (tokenList.size() == 1 and tokenList[tokenList.size() - 1].find(name_id) != -1) {
		place = 0;
	}
	return tokenList[place];
}
string make_place(string word) {
	int start, start2;
	int countSymbols;
	string name_id;
	start = word.find(" ") + 1;
	start2 = word.find(" ", start);
	if (start == start2) {
		countSymbols = word.length() - start;
		name_id.assign(word, start+1, countSymbols);
		return name_id;
	}
	start = word.find(" ", 3) + 1;
	countSymbols = word.length() - start;
	name_id.assign(word, start, countSymbols);
	return name_id;
}
int find_n(string name_id, string alphabet) {
	int n_search;
	for (int i = 0; i < name_id.size(); i++) { //якщо ввели літеру(літери)
		if (alphabet.find(name_id[i]) != -1) {
			return 0;
		}
	}
	if (name_id == "") { //якщо не ввели число
		return 0;
	}
	else {
		n_search = stoi(name_id);
		if (n_search < 0) { //якщо ввели від'ємне число
			n_search = 0;
		}
		return n_search;
	}
}
string filter_text(string txt){
	for (int i = 0; i < txt.size(); i++) {
		if (!isalpha((unsigned char)txt[i]) and (txt[i] != '1' and txt[i] != '2' and txt[i] != '3' and txt[i] != '4' and txt[i] != '5' and txt[i] != '6' and txt[i] != '7' and txt[i] != '8' and txt[i] != '9' and txt[i] != '0' and txt[i] != '_')) {
			txt[i] = ' ';
		}
	}
	return txt;
}

int main() {
	string txt;
	ifstream f;
	string command = "";
	Lexer lexer(command);
	Token token;

	string enter_command;
	string tokenForList;
	vector<string> tokenList;
	int start, countSymbols;
	string name_id, place_to_del;
	string keyword1, keyword2;
	int n_search;
	string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	bool check_table, insert_string, search_string, number_string, star_string;
	int counter_for_tables = 0;
	int number_of_table;
	vector<string> namesOfCollection;
	vector<Collection> collections;
	vector<InvertedIndex> indexTable;

	while (true) {
		getline(cin, enter_command, '\n');
		enter_command = replace_tabsAndSpaces(enter_command);
		command = command + enter_command + "\n";
		cout << "Listing of all commands: " << command << endl;
		if (enter_command[enter_command.length() - 1] != ';') {
			cout << "The ';' must be at the end of every command!" << endl;
		}
		else if (((enter_command.find("<") != -1 and enter_command.find(">") == -1) or (enter_command.find(">") != -1 and enter_command.find("<") == -1)) or (enter_command.find(">") < enter_command.find("<"))) {
			cout << "Incorrect entry of <>!" << endl;
		}
		else if ((count(enter_command.begin(), enter_command.end(), ';')) > 1) {
			cout << "The ';' must be ONLY at the end of our command!" << endl;
		}
		else {
			lexer.newWord(enter_command);
			n_search = 0;
			while ((token.lexer_get_next_token(lexer)) != false) {
				tokenForList = to_string(token.type) + " " + token.value + " " + to_string(token.position);
				tokenList.push_back(tokenForList);
				//cout << "Token(" << token.type << ", " << token.value << ", " << token.position << ")" << endl;

				switch (token.type) {
				case TOKEN_ID:
					//cout << "-ID" << endl;
					if (tokenList.size() >= 2) {
						if (tokenList[tokenList.size() - 2][0] - '0' == TOKEN_CREATE) { //create NAME ... (true/ERROR)
							//cout << "//create table//" << endl;
							start = enter_command.find(" ") + 1;
							if (enter_command.find(" ", start) == -1) { //для випадку create name (true)
								name_id = make_name(tokenList[tokenList.size() - 1]);
								check_table = true;
								for (int i = 0; i < counter_for_tables; i++) { //перебираємо усі наявні таблиці
									if (collections[i].getName() == name_id) { //якщо таблиця з назвою NAME вже існує (false)
										cout << "Table " << collections[i].getName() << " has already existed!" << endl;
										cout << "Try another name!" << endl;
										check_table = false;
									}
								}
								if (check_table == true) { //якщо це нова таблиця
									namesOfCollection.push_back(name_id);
									collections.push_back(namesOfCollection[counter_for_tables]); //create NAME (true)
									cout << "Collection " << collections[counter_for_tables].getName() << " has been created!" << endl;
									indexTable.push_back(namesOfCollection[counter_for_tables]);
									counter_for_tables++;
								}
								else { //якщо таблиця з назвою NAME вже існує (false)
									place_to_del = make_place(tokenList[tokenList.size() - 2]);
									lexer.deleteWords(stoi(place_to_del)); //в лексері прибираємо зайві токени
									tokenList.erase(tokenList.begin() + (tokenList.size() - 1)); //видаляємо останні два токени з історії токенів
									tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
								}
							}
						}
						else if (tokenList[tokenList.size() - 2][0] - '0' == TOKEN_INSERT) { //insert NAME ...
							//cout << "//insert in table//" << endl;
							start = enter_command.find(" ") + 1;
							if (enter_command.find(" ", start) == -1) {
								cout << "//ERROR 8: incorrect INSERT command syntax//" << endl; //insert NAME;
								place_to_del = make_place(tokenList[tokenList.size() - 2]);
								lexer.deleteWords(stoi(place_to_del)); //в лексері прибираємо зайві токени
								tokenList.erase(tokenList.begin() + (tokenList.size() - 1)); //видаляємо останні два токени з історії токенів
								tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
							}
							else {
								insert_string = false;
								name_id = make_name(tokenList[tokenList.size() - 1]);
								for (int i = 0; i < counter_for_tables; i++) {
									if (collections[i].getName() == name_id) { //якщо таблиця з назвою NAME існує (true)
										cout << "Table " << collections[i].getName() << " exists!" << endl;
										insert_string = true;
										number_of_table = i;
									}
								}
							}
						}
						else if (tokenList[tokenList.size() - 2][0] - '0' == TOKEN_INDEX) { //print_index NAME
							//cout << "//print index//" << endl;
							check_table = false;
							name_id = make_name(tokenList[tokenList.size() - 1]);

							start = enter_command.find(name_id);
							if ((start + name_id.size()) < enter_command.size() - 1) {
								cout << "//ERROR 18.2: incorrect PRINT_INDEX command syntax//" << endl; //print_index two or more
								name_id = make_name2(enter_command, tokenList);
								//cout << "To delete: " << name_id << endl;
								place_to_del = make_place(name_id);
								lexer.deleteWords(stoi(place_to_del));
								while (tokenList[tokenList.size() - 1] != name_id) {
									tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
								}
								tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
							}
							else {
								for (int i = 0; i < counter_for_tables; i++) {
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
								else { //якщо таблиця з назвою NAME не існує (false)
									cout << "ERROR: There is no table with this name!!" << endl;
									place_to_del = make_place(tokenList[tokenList.size() - 2]);
									lexer.deleteWords(stoi(place_to_del)); //в лексері прибираємо зайві токени
									tokenList.erase(tokenList.begin() + (tokenList.size() - 1)); //видаляємо останні два токени з історії токенів
									tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
								}
							}
						}
						else if (tokenList[tokenList.size() - 2][0] - '0' == TOKEN_SEARCH) { //search NAME ...
							//cout << "//search in table//" << endl;
							check_table = false;
							start = enter_command.find(" ") + 1;
							if (enter_command.find(" ", start) == -1) { //для випадку search NAME
								countSymbols = enter_command.find(";", start) - start;
								name_id.assign(enter_command, start, countSymbols);
								for (int i = 0; i < counter_for_tables; i++) {
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
								else { //якщо таблиця з назвою NAME не існує (false)
									cout << "ERROR: There is no table with this name!!" << endl;
									place_to_del = make_place(tokenList[tokenList.size() - 2]);
									lexer.deleteWords(stoi(place_to_del)); //в лексері прибираємо зайві токени
									tokenList.erase(tokenList.begin() + (tokenList.size() - 1)); //видаляємо останні два токени з історії токенів
									tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
								}
							}
							else { //для випадку search NAME ...
								search_string = false;
								name_id = make_name(tokenList[tokenList.size() - 1]);
								for (int i = 0; i < counter_for_tables; i++) {
									if (collections[i].getName() == name_id) { //якщо таблиця з назвою NAME існує (true)
										cout << "Table " << collections[i].getName() << " exists!" << endl;
										search_string = true;
										number_of_table = i;
									}
								}
							}
						}
						else {
							cout << "//ERROR 2: incorrect command syntax//" << endl; //команда NAME;
							name_id = make_name2(enter_command, tokenList);
							//cout << "To delete: " << name_id << endl;
							place_to_del = make_place(name_id);
							lexer.deleteWords(stoi(place_to_del));
							while (tokenList[tokenList.size() - 1] != name_id) {
								tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
							}
							tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
							//cout << "Current: " << tokenList[tokenList.size() - 1] << endl;
						}
					}
					else {
						cout << "//ERROR 1: incorrect command syntax//" << endl; //перша команда NAME;
						//cout << "To delete: " << tokenList[tokenList.size() - 1] << endl;
						place_to_del = make_place(tokenList[tokenList.size() - 1]);
						lexer.deleteWords(stoi(place_to_del));
						tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
					}
					break;
				case TOKEN_CREATE:
					//cout << "-Create" << endl;
					if (tokenList.size() > 1) {
						if (tokenList[tokenList.size() - 2][0] - '0' != TOKEN_SEMI) {
							cout << "//ERROR 3: incorrect CREATE command syntax//" << endl; //command + CREATE;
							name_id = make_name3(enter_command, tokenList);
							//cout << "To delete: " << name_id << endl;
							place_to_del = make_place(name_id);
							lexer.deleteWords(stoi(place_to_del));
							while (tokenList[tokenList.size() - 1] != name_id) {
								tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
							}
							tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
							//cout << "Current: " << tokenList[tokenList.size() - 1] << endl;
						}
					}
					break;
				case TOKEN_INSERT:
					//cout << "-Insert" << endl;
					if (tokenList.size() > 1) {
						if (tokenList[tokenList.size() - 2][0] - '0' != TOKEN_SEMI) {
							cout << "//ERROR 4: incorrect INSERT command syntax//" << endl; //name/string/command INSERT;
							name_id = make_name3(enter_command, tokenList);
							//cout << "To delete: " << name_id << endl;
							place_to_del = make_place(name_id);
							lexer.deleteWords(stoi(place_to_del));
							while (tokenList[tokenList.size() - 1] != name_id) {
								tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
							}
							tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
							//cout << "Current: " << tokenList[tokenList.size() - 1] << endl;
						}
					}
					break;
				case TOKEN_SEARCH:
					//cout << "-Search" << endl;
					break;
				case TOKEN_WHERE:
					//cout << "-Where" << endl;
					if (tokenList.size() > 2) {
						if ((tokenList[tokenList.size() - 2][0] - '0' != TOKEN_ID) or (tokenList[tokenList.size() - 3][0] - '0' != TOKEN_SEARCH)) {
							cout << "//ERROR 9: incorrect WHERE command syntax//" << endl; //command + command WHERE;
							name_id = make_name2(enter_command, tokenList);
							//cout << "To delete: " << name_id << endl;
							place_to_del = make_place(name_id);
							lexer.deleteWords(stoi(place_to_del));
							while (tokenList[tokenList.size() - 1] != name_id) {
								tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
							}
							tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
							//cout << "Current: " << tokenList[tokenList.size() - 1] << endl;
						}
					}
					else {
						cout << "//ERROR 10: incorrect SEARCH command syntax//" << endl; //перша команда search WHERE or WHERE;
						name_id = make_name2(enter_command, tokenList);
						//cout << "To delete: " << name_id << endl;
						place_to_del = make_place(name_id);
						lexer.deleteWords(stoi(place_to_del));
						//cout << "Current: " << tokenList[tokenList.size() - 1] << endl;
						while (tokenList[tokenList.size() - 1] != name_id) {
							tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
						}
						tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
					}
					break;
				case TOKEN_INDEX:
					//cout << "-Print_index" << endl;
					break;
				case TOKEN_SEMI:
					//cout << "-;" << endl;
					if (tokenList.size() > 1) {
						if ((tokenList[tokenList.size() - 2][0] - '0' != TOKEN_STRING) and (tokenList[tokenList.size() - 2][0] - '0' != TOKEN_ID) and (tokenList[tokenList.size() - 2][0] - '0' != TOKEN_STAR)) {
							cout << "//ERROR 7: incorrect command syntax//" << endl; //create; insert; ...
							name_id = make_name3(enter_command, tokenList);
							//cout << "To delete: " << name_id << endl;
							place_to_del = make_place(name_id);
							if (enter_command[0] == '\"' or enter_command[0] == '<') {
								//cout << "To delete: " << stoi(place_to_del) - 1 << endl;
								lexer.deleteWords(stoi(place_to_del) - 1);
							}
							else {
								lexer.deleteWords(stoi(place_to_del));
							}
							while (tokenList[tokenList.size() - 1] != name_id) {
								tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
							}
							tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
						}
					}
					break;
				case TOKEN_STRING:
					//cout << "-String" << endl;
					if (n_search == 0) {
						if (tokenList.size() > 2) {
							if ((tokenList[tokenList.size() - 2][0] - '0' == TOKEN_ID) and (tokenList[tokenList.size() - 3][0] - '0' == TOKEN_INSERT)) {
								if (insert_string == true) { //якщо таблиця з назвою NAME існує (true) // insert name "VALUE"
									name_id = make_name(tokenList[tokenList.size() - 1]);
									txt = inputf(f, name_id);
									txt = filter_text(txt);
									txt = replace_tabsAndSpaces(txt);
									//cout << "txt1: " << txt << endl;

									if (txt == "error") {
										cout << "Fail to open the file. Write correct root to your .txt file!" << endl;
										txt = "";
										place_to_del = make_place(tokenList[tokenList.size() - 3]);
										lexer.deleteWords(stoi(place_to_del)); //в лексері прибираємо зайві токени
										tokenList.erase(tokenList.begin() + (tokenList.size() - 1)); //видаляємо останні три токени з історії токенів
										tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
										tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
									}
									else {
										start = enter_command.find(name_id);
										if ((start + name_id.size()) < enter_command.size()-2) {
											cout << "//ERROR 18.1: incorrect INSERT command syntax//" << endl; //insert two or more
											name_id = make_name2(enter_command, tokenList);
											//cout << "To delete: " << name_id << endl;
											place_to_del = make_place(name_id);
											lexer.deleteWords(stoi(place_to_del));
											while (tokenList[tokenList.size() - 1] != name_id) {
												tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
											}
											tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
										}
										else {
											collections[number_of_table].insertDocument(txt, name_id);
											cout << "Document has been added to " << collections[number_of_table].getName() << endl;
											collections[number_of_table].wordsForInvertedIndex(indexTable[number_of_table], collections[number_of_table].documents.size() - 1);
										}
									}
								}
								else { //якщо таблиця з назвою NAME не існує (false)
									cout << "ERROR: There is no table with this name!!" << endl;
									place_to_del = make_place(tokenList[tokenList.size() - 3]);
									lexer.deleteWords(stoi(place_to_del)); //в лексері прибираємо зайві токени
									tokenList.erase(tokenList.begin() + (tokenList.size() - 1)); //видаляємо останні три токени з історії токенів
									tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
									tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
								}
							}
							else if ((tokenList[tokenList.size() - 2][0] - '0' == TOKEN_WHERE) and (tokenList[tokenList.size() - 3][0] - '0' == TOKEN_ID) and (tokenList[tokenList.size() - 4][0] - '0' == TOKEN_SEARCH)) {
								//cout << "//search in table where \" \"//" << endl;

								if (search_string == true) { //якщо таблиця з назвою NAME існує (true)
									check_table = true;
									number_string = false;
									star_string = false;
									start = enter_command.find("\"");
									if (enter_command.find("*", start) != -1) {
										check_table = false;
									}
									name_id = make_name(tokenList[tokenList.size() - 1]);
									if (check_table == true) {
										if (enter_command.find("<", start) != -1) {
											number_string = true;
										}
										if (number_string == false) {
											start = enter_command.find(name_id);
											if ((start + name_id.size()) < enter_command.size() - 2) {
												cout << "//ERROR 18.3: incorrect SEARCH command syntax//" << endl; //search two or more
												name_id = make_name3(enter_command, tokenList);
												//cout << "To delete: " << name_id << endl;
												place_to_del = make_place(name_id);
												lexer.deleteWords(stoi(place_to_del));
												while (tokenList[tokenList.size() - 1] != name_id) {
													tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
												}
												tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
											}
											else {
												if (collections[number_of_table].documents.size() == 0) {
													cout << "Table " << collections[number_of_table].getName() << " is empty!" << endl;
												}
												else {
													indexTable[number_of_table].search_keyword(name_id, collections[number_of_table].nameOfDocuments, collections[number_of_table].documents);
												}
											}
										}
										else {
											//cout << "With <>" << endl;
											keyword1 = name_id;
										}
									}
									else {
										if (enter_command.find("<", start) != -1) {
											number_string = true;
										}
										if (number_string == false) {
											star_string = true;
											//cout << "With *" << endl;
											for (int i = 0; i < enter_command.length(); i++) {
												if (enter_command[i] == '*') {
													start = i;
												}
											}
											if (enter_command[(start + 1)] != ';') {
												cout << "//ERROR 19: incorrect query syntax//" << endl; //search name where "string*"
												name_id = make_name3(enter_command, tokenList);
												//cout << "To delete: " << name_id << endl;
												place_to_del = make_place(name_id);
												lexer.deleteWords(stoi(place_to_del));
												while (tokenList[tokenList.size() - 1] != name_id) {
													tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
												}
												tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
											}
											else {
												keyword1 = name_id;
											}
										}
										else {
											cout << "//ERROR 11: incorrect SEARCH command syntax//" << endl; //search ... * and <>;
											place_to_del = make_place(tokenList[tokenList.size() - 4]);
											lexer.deleteWords(stoi(place_to_del)); //в лексері прибираємо зайві токени
											tokenList.erase(tokenList.begin() + (tokenList.size() - 1)); //видаляємо останні 4 токени з історії токенів
											tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
											tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
											tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
										}
									}
								}
								else { //якщо таблиця з назвою NAME не існує (false)
									cout << "ERROR: There is no table with this name!!" << endl;
									place_to_del = make_place(tokenList[tokenList.size() - 4]);
									lexer.deleteWords(stoi(place_to_del)); //в лексері прибираємо зайві токени
									tokenList.erase(tokenList.begin() + (tokenList.size() - 1)); //видаляємо останні 4 токени з історії токенів
									tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
									tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
									tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
								}
							}
							else {
								cout << "//ERROR 6: incorrect command syntax//" << endl; //команда STRING or command STRING;
								name_id = make_name3(enter_command, tokenList);
								//cout << "To delete: " << name_id << endl;
								place_to_del = make_place(name_id);
								if (enter_command[0] == '\"') {
									//cout << "To delete: " << stoi(place_to_del) - 1 << endl;
									lexer.deleteWords(stoi(place_to_del) - 1);
								}
								else {
									lexer.deleteWords(stoi(place_to_del));
								}
								while (tokenList[tokenList.size() - 1] != name_id) {
									tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
								}
								tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
								//cout << "Current: " << tokenList[tokenList.size() - 1] << endl;
							}
						}
						else {
							cout << "//ERROR 5: incorrect command syntax//" << endl; //перша команда STRING or command STRING;
							name_id = make_name3(enter_command, tokenList);
							//cout << "To delete: " << name_id << endl;
							place_to_del = make_place(name_id);
							if (enter_command[0] == '\"') {
								//cout << "To delete: " << stoi(place_to_del) - 1 << endl;
								lexer.deleteWords(stoi(place_to_del) - 1);
							}
							else {
								lexer.deleteWords(stoi(place_to_del));
							}
							//cout << "Current: " << tokenList[tokenList.size() - 1] << endl;
							while (tokenList[tokenList.size() - 1] != name_id) {
								tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
							}
							tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
						}
					}
					else { //search name where string <> STRING
						if ((tokenList[tokenList.size() - 2][0] - '0' == TOKEN_NUMBER) and (tokenList[tokenList.size() - 3][0] - '0' == TOKEN_STRING) and (tokenList[tokenList.size() - 4][0] - '0' == TOKEN_WHERE) and (tokenList[tokenList.size() - 5][0] - '0' == TOKEN_ID) and (tokenList[tokenList.size() - 6][0] - '0' == TOKEN_SEARCH)) {
							keyword2 = make_name(tokenList[tokenList.size() - 1]);

							start = enter_command.find(keyword2);
							if ((start + keyword2.size()) < enter_command.size() - 2) {
								cout << "//ERROR 18.4: incorrect SEARCH command syntax//" << endl; //search <> two or more
								name_id = make_name3(enter_command, tokenList);
								//cout << "To delete: " << name_id << endl;
								place_to_del = make_place(name_id);
								lexer.deleteWords(stoi(place_to_del));
								while (tokenList[tokenList.size() - 1] != name_id) {
									tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
								}
								tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
							}
							else {
								indexTable[number_of_table].search_two_key(keyword1, keyword2, n_search, collections[number_of_table].nameOfDocuments, collections[number_of_table].documents);
							}
						}
						else {
							cout << "//ERROR 17//" << endl; //неправильний порядок команд
						}
					}
					
					break;
				case TOKEN_STAR:
					//cout << "-*" << endl;
					if (tokenList.size() > 1) {
						if (tokenList[tokenList.size() - 2][0] - '0' != TOKEN_STRING) {
							cout << "//ERROR 12: incorrect command syntax//" << endl; // команда *;
							name_id = make_name2(enter_command, tokenList);
							//cout << "To delete: " << name_id << endl;
							place_to_del = make_place(name_id);
							lexer.deleteWords(stoi(place_to_del));
							while (tokenList[tokenList.size() - 1] != name_id) {
								tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
							}
							tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
							//cout << "Current: " << tokenList[tokenList.size() - 1] << endl;
						}
						else {
							if (star_string == true) {
								if (collections[number_of_table].documents.size() == 0) {
									cout << "Table " << collections[number_of_table].getName() << " is empty!" << endl;
								}
								else {
									indexTable[number_of_table].search_prefix(keyword1, collections[number_of_table].nameOfDocuments, collections[number_of_table].documents);
								}
							}
						}
					}
					else {
						cout << "//ERROR 13: incorrect command syntax//" << endl; //перша команда *;
						//cout << "To delete: " << tokenList[tokenList.size() - 1] << endl;
						place_to_del = make_place(tokenList[tokenList.size() - 1]);
						lexer.deleteWords(stoi(place_to_del));
						tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
					}
					break;
				case TOKEN_NUMBER:
					//cout << "-<n>" << endl;
					name_id = make_name(tokenList[tokenList.size() - 1]);
					n_search = find_n(name_id, alphabet);
					//cout << "-<" << n_search << ">" << endl;
					if (tokenList.size() >= 5) {
						if ((tokenList[tokenList.size() - 2][0] - '0' != TOKEN_STRING) or (tokenList[tokenList.size() - 3][0] - '0' != TOKEN_WHERE) or (tokenList[tokenList.size() - 4][0] - '0' != TOKEN_ID) or (tokenList[tokenList.size() - 5][0] - '0' != TOKEN_SEARCH)) {
							cout << "//ERROR 14: incorrect command syntax//" << endl; //неправильні команди із <>
							name_id = make_name3(enter_command, tokenList);
							//cout << "To delete: " << name_id << endl;
							place_to_del = make_place(name_id);
							lexer.deleteWords(stoi(place_to_del));
							while (tokenList[tokenList.size() - 1] != name_id) {
								tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
							}
							tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
							//cout << "Current: " << tokenList[tokenList.size() - 1] << endl;
						}
						else if (n_search == 0) {
							cout << "//ERROR 15: incorrect query syntax//" << endl; //неправильний запис всередині <>
							//cout << "To delete: " << tokenList[tokenList.size() - 5] << endl;
							place_to_del = make_place(tokenList[tokenList.size() - 5]);
							lexer.deleteWords(stoi(place_to_del));
							tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
							tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
							tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
							tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
							tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
						}
					}
					else {
						cout << "//ERROR 16: incorrect command syntax//" << endl; //перша команда <>
						name_id = make_name3(enter_command, tokenList);
						//cout << "To delete: " << name_id << endl;
						place_to_del = make_place(name_id);
						if (enter_command[0] == '<') {
							//cout << "To delete: " << stoi(place_to_del) - 1 << endl;
							lexer.deleteWords(stoi(place_to_del) - 1);
						}
						else {
							lexer.deleteWords(stoi(place_to_del));
						}
						while (tokenList[tokenList.size() - 1] != name_id) {
							tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
						}
						tokenList.erase(tokenList.begin() + (tokenList.size() - 1));
					}
					break;
				case TOKEN_EXIT:
					exit(1);
					break;
				}
			}
		}
	}
}
