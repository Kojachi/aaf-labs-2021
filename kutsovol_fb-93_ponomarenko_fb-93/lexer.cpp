#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <cstdlib>
#include <list>
#include <cctype>
#include <regex>

using namespace std;

typedef struct Token {
	int type;
	string value;
	int position;
	Token(int type_, string value_, int pos_) {
		type = type_;
		value = value_;
		position = pos_;
	}
} token_T;
enum Tokens {
	TOKEN_ID, //0
	TOKEN_CREATE, //1
	TOKEN_INSERT, //2
	//TOKEN_DELETE,
	TOKEN_SEARCH, //3
	TOKEN_WHERE, //4
	TOKEN_LPAREN, //5
	TOKEN_RPAREN, //6
	TOKEN_SEMI, //7
	TOKEN_STRING //8
} type;
string token_types_array[] = {
		{"[cC][rR][eE][aA][tT][eE]"},
		{"[iI][nN][sS][eE][rR][tT]"},
		//{"[iI][nN][sS][eE][rR][tT]([ \t\n\r]{0,}[iI][nN][tT][oO])?"},
		//{"[dD][eE][lL][eE][tT][eE]([ \t\n\r]{0,}[fF][rR][oO][mM])?"},
		{"[sS][eE][aA][rR][cC][hH]"},
		{"[wW][hH][eE][rR][eE]"}
};
int token_types_size = sizeof(token_types_array) / sizeof(token_types_array[0]);
int pos;

token_T* init_token(Tokens type, string value, int pos) {
	token_T* token = new token_T(type, value, pos);
	token->type = type;
	token->value = value;
	token->position = pos;
	return token;
}

typedef struct Lexer {
	string c;
	unsigned int i;
	string contents;
} lexer_T;

lexer_T* init_lexer(string contents) {
	//lexer_T* lexer = (lexer_T*)calloc(1, sizeof(Lexer));
	lexer_T* lexer = new lexer_T;
	lexer->contents = contents;
	lexer->i = 0;
	lexer->c = contents[lexer->i];
	return lexer;
}
void lexer_advance(lexer_T* lexer) {
	if (lexer->i < (lexer->contents).length()) {
		lexer->i += 1;
		lexer->c = lexer->contents[lexer->i];
	}
}
void skip_space(lexer_T* lexer) {
	while (lexer->c == " " or lexer->c == "\n") {
		lexer_advance(lexer);
	}
}
token_T* lexer_advance_with_token(lexer_T* lexer, token_T* token) {
	lexer_advance(lexer);
	return token;
}
string lexer_get_current_string(lexer_T* lexer) {
	string str;
	str = lexer->c;
	return str;
}
token_T* lexer_collect_string(lexer_T* lexer) {
	lexer_advance(lexer);
	int pos;
	pos = lexer->i;
	string value = "";
	while (lexer->c != "\"") {
		string s = lexer_get_current_string(lexer);
		value = value + s;
		lexer_advance(lexer);
	}
	lexer_advance(lexer);
	return init_token(TOKEN_STRING, value, pos);
}
token_T* lexer_collect_id(lexer_T* lexer) {
	string value = "";
	int pos;
	pos = lexer->i;
	while ((isalnum(lexer->c[0], locale())) or (lexer->c == "_")) {
		string s = lexer_get_current_string(lexer);
		value = value + s;
		lexer_advance(lexer);
	}
	//if value == CREATE...
	smatch matches;
	//string newValue(value);
	for (int i = 0; i < token_types_size; i++) {
		regex regular("^" + token_types_array[i]);
		if (regex_search(value, matches, regular))
		{
			if (token_types_array[i] == "[cC][rR][eE][aA][tT][eE]") {
				//cout << "Collection has been created" << endl;
				//if: CREATE + !=TOKEN_ID => ERROR
				return lexer_advance_with_token(lexer, init_token(TOKEN_CREATE, value, pos));
			}
			if (token_types_array[i] == "[iI][nN][sS][eE][rR][tT]") {
				//cout << "Document has been added to our collection" << endl;
				//if: INSERT + !=TOKEN_ID + !=TOKEN_STRING => ERROR
				return lexer_advance_with_token(lexer, init_token(TOKEN_INSERT, value, pos));
			}
			////if: SEARCH + !=TOKEN_ID or SEARCH + !=TOKEN_ID + !=WHERE + !=TOKEN_STRING => ERROR
			if (token_types_array[i] == "[sS][eE][aA][rR][cC][hH]") {
				return lexer_advance_with_token(lexer, init_token(TOKEN_SEARCH, value, pos));
			}
			if (token_types_array[i] == "[wW][hH][eE][rR][eE]") {
				return lexer_advance_with_token(lexer, init_token(TOKEN_WHERE, value, pos));
			}
		}
	}
	return init_token(TOKEN_ID, value, pos);
}
token_T* lexer_get_next_token(lexer_T* lexer) {
	if (lexer->i < (lexer->contents).length()) {
		pos = lexer->i;
	}
	while (lexer->i < (lexer->contents).length()) {
		if (lexer->c == " " or lexer->c == "\n") {
			skip_space(lexer);
		}
		if (lexer->c == "\"") {
			return lexer_collect_string(lexer);
		}
		if (isalnum(lexer->c[0], locale())) {
			return lexer_collect_id(lexer);
		}
		switch (lexer->c[0]) {
		case ';':
			return lexer_advance_with_token(lexer, init_token(TOKEN_SEMI, lexer_get_current_string(lexer), pos));
			break;
		case '(':
			return lexer_advance_with_token(lexer, init_token(TOKEN_LPAREN, lexer_get_current_string(lexer), pos));
			break;
		case ')':
			return lexer_advance_with_token(lexer, init_token(TOKEN_RPAREN, lexer_get_current_string(lexer), pos));
			break;
		}
	}
	return NULL;
}

struct Collection {
	string name;
	//vector<string> documents;
	//vector<string> nameOfDocuments;
	string getName();
	//vector<string> words1;
	//vector<string> words2;
	//vector<string> words3;
	//int rows = 3;
	//int columns;
	//string** documents_collection = new string * [rows];
	//vector<vector<string>> wordsM;
	//void insertDocument(string txt);
	//void id_document(char name[40]);
	//void readDocuments();
	//void read_id_document();
	//void makeWords();
	//void readWords();
	Collection(string name_) {
		name = name_;
	}
};
string Collection::getName() {
	return name;
}

int main() {
	setlocale(LC_ALL, "Russian");
	string enter;
	vector<string> namesOfCollection;
	vector<Collection> collections;
	string nameOfCollection;
	int counter = 0;
	cout << "...Creation of the collection..." << endl;
	cout << "\"CREATE name;\".: ";
	getline(cin, nameOfCollection, '.');
	lexer_T* lexer = init_lexer(nameOfCollection);
	token_T* token;
	vector<string> tokenList;
	string tokenForList;

	string name_id;
	int start, countSymbols;

	while ((token = lexer_get_next_token(lexer)) != NULL) {
		tokenForList = to_string(token->type) + " " + token->value + " " + to_string(token->position);
		tokenList.push_back(tokenForList);
		cout << "Token(" << token->type << ", " << token->value << ", " << token->position << ")" << endl;
	}
	
	if ((((tokenList[0])[0]-'0') == TOKEN_CREATE) and (((tokenList[1])[0]-'0') == TOKEN_ID) and (((tokenList[tokenList.size() - 1])[0] -'0') == TOKEN_SEMI)) {
		start = (tokenList[1])[tokenList[1].length() - 1]-'0';
		countSymbols = nameOfCollection.find(";") - start;
		name_id.assign(nameOfCollection, start, countSymbols);
		//cout << name_id << endl;
		namesOfCollection.push_back(name_id);
		collections.push_back(namesOfCollection[counter]);
		cout << "Collection " << collections[counter].getName() << " has been created!" << endl;
		counter++;
	}
	else if((((tokenList[0])[0] - '0') == TOKEN_CREATE) and (((tokenList[1])[0] - '0') == TOKEN_ID) and (((tokenList[tokenList.size()-1])[0] - '0') != TOKEN_SEMI)) {
		cout << "ERROR" << endl;
		cout << "You don't use the \";\"" << endl;
	}
	else {
		cout << "ERROR" << endl;
	}
	//введення команд користувачем
	getline(cin, enter, '.');
	enter.erase(0, 1);
	lexer_T* lexer_d = init_lexer(enter);
	token_T* token_d;
	vector<string> tokenList_d;
	string tokenForList_d;
	//розбиття на токени
	while ((token_d = lexer_get_next_token(lexer_d)) != NULL) {
		tokenForList_d = "(" + to_string(token_d->type) + " " + token_d->value + " " + to_string(token_d->position) + ")";
		tokenList_d.push_back(tokenForList_d);
		cout << "Token(" << token_d->type << ", " << token_d->value << ", " << token_d->position << ")" << endl;
	}
}
