//Nitulescu Ion Alexandru, gupa 341

#include <iostream>
#include <fstream>
#include <map>
#include <string.h>
#include <string>
#include<stdio.h>
#include<stdlib.h>
#include <vector>
#include<algorithm>
#define BUFFER_SIZE 4096;
using namespace std;
int characters;
FILE *file;
//char input[256] = "input.txt";
int number_of_tokens;

enum GLOBAL {
	END_OF_FILE = -1,
	INVALID,
	KEYWORD,
	IDENTIFIER,
	INT,
	FLOAT,
	DOUBLE,
	CHAR,
	STRING,
	ARITHMETIC_OPERATOR,
	INCREMENT,
	ASSIGNMENT,
	RELATIONAL,
	LOGICAL,
	SEPARATOR,
	COMMENT,
	SPACE,
	MEMBER_SELECTION,
	UNKNOWN,
	BITS_OPERATOR,
	NEGATION,
	LETTER,
	STAR,
	NUMBER,
	NEW_LINE,
	SLASH
};

enum STATE {
	INITIAL,
	FINAL,
	ERROR
};


//validate functions:
bool isBackSlash(char c)
{
	return (c == '\\');
}

bool isDigit(char c)
{
	return (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' || c == '0');
}

bool isAnythingButBackslashAndQuotationMark(char c) {
	return (c != '\\' && c != '"');
}
bool isLetter(char c)
{
	return (c >= 'a'&&c <= 'z') || (c >= 'A'&&c <= 'Z');
}

bool isLetterOrUnderscore(char c)
{
	return (isLetter(c) || c == '_');
}

bool isLetterOrUnderscoreOrDigit(char c)
{
	return (isLetterOrUnderscore(c) || isdigit(c));
}

bool isDigitOrFloat(char c)
{
	return (isDigit(c) || c == 'f');		//f for float numbers
}

bool isFloat(char c)
{
	return (c == 'f');
}

bool isEOL(char c)			//end of line
{
	return (c == '\n');
}

bool isEOLorEOF(char c)			//end of line or end of file
{
	return (c == '\n' || c == '\0');
}

bool isNotEOF(char c)
{
	return (!(isEOL(c)));
}

bool isNotEndOfLineAndBackslash(char c)
{
	return (!(isEOL(c)) && !(isBackSlash(c)));
}

bool isNotEndOfLineAndBackslashAndEndOfFile(char c)
{
	return (isNotEOF(c) && isNotEndOfLineAndBackslash(c));
}

bool isE(char c)		//E is for float numbers
{
	return (c == 'e' || c == 'E');
}

bool isSpace(char c)
{
	return (c == ' ' || c == '\n');
}

bool isDot(char c)
{
	return (c == '.');
}

bool isComma(char c)
{
	return (c == ',');
}

bool isColon(char c)
{
	return (c == ':');
}

bool is_separator(char c)
{
	switch (c)
	{
	case ';':
	case ',':
	case '{':
	case '}':
	case ']':
	case ')':
	case '(':
	case '[':
		return true;
	default:
		return false;
	}
}

bool isApostrophe(char c)
{
	return (c == '\'');
}

bool isQuotationMark(char c)
{
	return (c == '"');
}

bool isSlash(char c)
{
	return (c == '/');
}

bool isNotSlash(char c)
{
	return (!isSlash(c));
}

bool isStar(char c)
{
	return (c == '*');
}

bool isEscapedCharacter(char c)
{
	return  c == '"' || c == '\'' || c == '?' || c == '\\';
}

bool isLesserThan(char c)
{
	return (c == '<');
}

bool isGreaterThan(char c)
{
	return (c == '>');
}

bool isGreaterOrLesserThanSign(char c)
{
	return (c == '<' || c == '>');
}

bool isEqualSign(char c)
{
	return (c == '=');
}

bool isAdditionOrSubstractionSign(char c)
{
	return (c == '+' || c == '-');
}

bool isAdditionSign(char c)
{
	return (c == '+');
}

bool isSubstractionSign(char c)
{
	return (c == '-');
}

bool isModulusOrMultiplicationSign(char c)
{
	return (c == '%' || c == '*');
}

bool isModulus(char c)
{
	return (c == '%');
}

bool isPipe(char c)
{
	return (c == '|');
}

bool isAmpersand(char c)
{
	return (c == '&');
}

bool isCaret(char c)
{
	return (c == '^');
}

bool isTilda(char c)
{
	return (c == '~');
}

bool isCarretOrTilda(char c)
{
	return (isCaret(c) || isTilda(c));
}

bool isQuestionMark(char c)
{
	return (c == '?');
}

bool isNegationSign(char c)
{
	return (c == '!');
}

bool isSpecialKeyword(string s)
{
	vector<string> words;
	words.push_back("case");
	words.push_back("char");
	words.push_back("const");
	words.push_back("continue");
	words.push_back("default");
	words.push_back("do");
	words.push_back("double");
	words.push_back("break");
	words.push_back("else");
	words.push_back("enum");
	words.push_back("float");
	words.push_back("for");
	words.push_back("if");
	words.push_back("int");
	words.push_back("long");
	words.push_back("register");
	words.push_back("return");
	words.push_back("short");
	words.push_back("signed");
	words.push_back("sizeof");
	words.push_back("static");
	words.push_back("struct");
	words.push_back("switch");
	words.push_back("main");
	words.push_back("typedef");
	words.push_back("unsigned");
	words.push_back("void");
	words.push_back("while");
	words.push_back("return");
	words.push_back("argc");
	words.push_back("argv");
	if (find(words.begin(), words.end(), s) != words.end())
		return true;

	return false;
}

bool isAnythingButBackSlashAndAstrophe(char c)
{
	return !isBackSlash(c) && !isApostrophe(c);
}
bool isComment(char c)
{
	return c == '/';
}
bool isEOF(char c)
{
	return c == '\0';
}
struct Token {
	int type, value;
	string message;
	Token(int, int, string);
};

Token::Token(int t = 0, int v = 0, string m = "")
{
	type = t;
	value = v;
	message = m;
}
struct Node {

	vector<int> edges;		//the adiacent edges

	int type;	//state type
	string errMsg;

	vector<bool(*)(char)> isValid;		//verifies if you can go to another state
	vector<bool(*)(string)> isValidForStrings;
};



class DFA {
public:
	DFA();			//constructor
	vector<Node> nodesOfDFA;			//the DFA states
	int maxEdgeIndex = 0;
	void makeEdge(int first, int second, bool(*func)(char))		//create an edge from the first int to the second if the condition is true
	{
		nodesOfDFA[first].edges.push_back(second);
		nodesOfDFA[first].isValid.push_back(func);
		if (first > maxEdgeIndex)
		{
			maxEdgeIndex = first;
		}
		if (second > maxEdgeIndex)
		{
			maxEdgeIndex = second;
		}

	}
	void makeEdge(int first, int second, bool(*func)(string))
	{
		nodesOfDFA[first].edges.push_back(second);
		nodesOfDFA[first].isValidForStrings.push_back(func);

	}
};

DFA::DFA()
{
	//the edges of the dfa
	for (int i = 0; i < 100; i++)
		nodesOfDFA.push_back(Node());

	makeEdge(INITIAL, 1, isLetterOrUnderscore);		nodesOfDFA[1].type = LETTER;
	makeEdge(INITIAL, 2, isSpace);					nodesOfDFA[2].type = SPACE;
	makeEdge(INITIAL, 3, isLetter);
	makeEdge(INITIAL, 4, isApostrophe);				nodesOfDFA[4].type = CHAR;
	makeEdge(INITIAL, 8, isQuotationMark);			nodesOfDFA[8].type = SEPARATOR;
	makeEdge(INITIAL, 11, is_separator);			nodesOfDFA[11].type = SEPARATOR;
	makeEdge(INITIAL, 12, isDigit);					nodesOfDFA[12].type = INT;
	makeEdge(INITIAL, 19, isSlash);					nodesOfDFA[19].type = SLASH;
	makeEdge(INITIAL, 25, isAdditionSign);			nodesOfDFA[25].type = ARITHMETIC_OPERATOR;
	makeEdge(INITIAL, 26, isSubstractionSign);	    nodesOfDFA[26].type = ARITHMETIC_OPERATOR;
	makeEdge(INITIAL, 28, isModulus);				nodesOfDFA[28].type = ARITHMETIC_OPERATOR;
	makeEdge(INITIAL, 30, isEqualSign);				nodesOfDFA[30].type = ASSIGNMENT;
	makeEdge(INITIAL, 31, isGreaterThan);			nodesOfDFA[31].type = RELATIONAL;
	makeEdge(INITIAL, 33, isAmpersand);				nodesOfDFA[33].type = BITS_OPERATOR;
	makeEdge(INITIAL, 37, isLesserThan);			nodesOfDFA[37].type = RELATIONAL;
	makeEdge(INITIAL, 35, isPipe);					nodesOfDFA[35].type = SEPARATOR;
	makeEdge(INITIAL, 36, isNegationSign);			nodesOfDFA[36].type = NEGATION;
	makeEdge(INITIAL, 39, isQuestionMark);			nodesOfDFA[39].type = SEPARATOR;
	makeEdge(INITIAL, 38, isColon);					nodesOfDFA[38].type = SEPARATOR;
	makeEdge(INITIAL, 40, isStar);					nodesOfDFA[40].type = STAR;
	makeEdge(INITIAL, 41, isEOF);					nodesOfDFA[41].type = END_OF_FILE;
	makeEdge(INITIAL, 43, isEOL);					nodesOfDFA[43].type = NEW_LINE;
	makeEdge(INITIAL, 44, isBackSlash);				nodesOfDFA[44].type = SEPARATOR;
	makeEdge(1, 3, isLetter);
	makeEdge(3, 3, isLetter);						 nodesOfDFA[3].type = STRING;
	makeEdge(2, 2, isSpace);
	makeEdge(4, 5, isBackSlash);
	makeEdge(5, 6, isEscapedCharacter);				nodesOfDFA[5].errMsg = "ERR";
	makeEdge(4, 6, isAnythingButBackSlashAndAstrophe);
	makeEdge(6, 7, isApostrophe);					nodesOfDFA[7].type = CHAR;
	makeEdge(12, 13, isFloat);						nodesOfDFA[13].type = FLOAT;
	makeEdge(12, 14, isDot);						nodesOfDFA[14].type = FLOAT;
	makeEdge(14, 15, isDigit);						nodesOfDFA[15].type = FLOAT;
	makeEdge(15, 15, isDigit);
	makeEdge(15, 16, isAdditionOrSubstractionSign);
	makeEdge(16, 17, isDigit);						nodesOfDFA[17].type = FLOAT;
	makeEdge(17, 17, isDigit);
	makeEdge(19, 20, isSlash);						nodesOfDFA[20].type = COMMENT;
	makeEdge(20, 20, isSlash);
	makeEdge(25, 27, isAdditionSign);				nodesOfDFA[27].type = INCREMENT;
	makeEdge(26, 27, isSubstractionSign);
	makeEdge(25, 29, isEqualSign);
	makeEdge(26, 29, isEqualSign);					nodesOfDFA[29].type = ASSIGNMENT;
	makeEdge(31, 32, isEqualSign);					nodesOfDFA[32].type = RELATIONAL;
	makeEdge(37, 32, isEqualSign);
	makeEdge(36, 46, isEqualSign);					nodesOfDFA[46].type = RELATIONAL;
	makeEdge(33, 34, isAmpersand);					nodesOfDFA[34].type = BITS_OPERATOR;
	makeEdge(37, 46, isLesserThan);					nodesOfDFA[46].type = BITS_OPERATOR;
	makeEdge(31, 38, isGreaterThan);				nodesOfDFA[38].type = BITS_OPERATOR;
	makeEdge(12, 45, isDigit);						nodesOfDFA[45].type = NUMBER;

}


struct Scanner {
	DFA dfa;
	map<string, int> stringsTable;
	int bufferPosition;
	Scanner();
	~Scanner();
	Token getToken();
	int bufferSize;
	char* text;
};
Scanner::Scanner()
{
	bufferPosition = -1;
	bufferSize = BUFFER_SIZE;
	text = new char[bufferSize];
}

Token Scanner::getToken()
{
	Token t;
	int currentState = INITIAL;
	t.message = "";
	int i;

	if (bufferPosition >= bufferSize)
	{
		exit(0);
	}

	bufferPosition++;
	if (text[bufferPosition] == ' ' || text[bufferPosition] == '\n')
	{
		while (text[bufferPosition] == ' ' || text[bufferPosition] == '\n')
		{
			bufferPosition++;
		}
	}

	for (i = 0; i < dfa.nodesOfDFA[currentState].edges.size(); i++)
	{
		if (dfa.nodesOfDFA[currentState].isValid[i](text[bufferPosition]))
		{
			t.message += text[bufferPosition];
			currentState = dfa.nodesOfDFA[currentState].edges[i];
			t.type = dfa.nodesOfDFA[currentState].type;
			bufferPosition++;
			i--;

			if (stringsTable.count(t.message) > 0)
			{
				t.value = stringsTable.find(t.message)->second;
			}
			else
			{
				t.value = stringsTable.size();
				pair<string, int> pair = make_pair(t.message, t.value);
				stringsTable.insert(pair);
			}

			if (t.type == STRING && isSpecialKeyword(t.message))
			{
				t.type = KEYWORD;
				break;
			}

			if (i == dfa.maxEdgeIndex)
			{
				currentState == INVALID;
			}
		}
	}


	if (t.type == INVALID || t.type == STAR || t.type == SEPARATOR || t.type == KEYWORD)
	{
		bufferPosition--;
	}
	return t;
}


Scanner::~Scanner()
{
	delete[] text;
}



map<int, string> createMapForTokens()
{
	map<int, string> map;
	map.insert(make_pair(END_OF_FILE, "END OF FILE"));
	map.insert(make_pair(INVALID, "INVALID"));
	map.insert(make_pair(IDENTIFIER, "IDENTIFIER"));
	map.insert(make_pair(INT, "INT"));
	map.insert(make_pair(KEYWORD, "KEYWORD"));
	map.insert(make_pair(FLOAT, "FLOAT"));
	map.insert(make_pair(DOUBLE, "DOUBLE"));
	map.insert(make_pair(LETTER, "LETTER"));
	map.insert(make_pair(CHAR, "CHAR"));
	map.insert(make_pair(ARITHMETIC_OPERATOR, "ARITHMETIC OPERATOR"));
	map.insert(make_pair(LOGICAL, "LOGICAL OPERATOR"));
	map.insert(make_pair(SEPARATOR, "SEPARATOR"));
	map.insert(make_pair(NUMBER, "NUMBER"));
	map.insert(make_pair(STRING, "STRING"));
	map.insert(make_pair(SPACE, "SPACE"));
	map.insert(make_pair(SLASH, "SLASH"));
	map.insert(make_pair(INCREMENT, "INCREMENT"));
	map.insert(make_pair(MEMBER_SELECTION, "MEMBER SELECTION"));
	map.insert(make_pair(UNKNOWN, "UNKNOWN"));
	map.insert(make_pair(RELATIONAL, "RELATIONAL OPERATOR"));
	map.insert(make_pair(ASSIGNMENT, "ASSIGNMENT OPERATOR"));
	map.insert(make_pair(BITS_OPERATOR, "BITS OPERATOR"));
	map.insert(make_pair(STAR, "STAR"));
	map.insert(make_pair(NEGATION, "NEGATION SIGN"));
	map.insert(make_pair(NEW_LINE, "END OF LINE"));
	map.insert(make_pair(COMMENT, "COMMENT"));
	return map;
}

int main(int argc, char* argv[])
{
	map<int, string> token_types = createMapForTokens();
	Scanner scanner;
	char *input, *output;
	input = argv[1];
	output = argv[2];
	file = fopen(input, "r");
	ofstream fout(output);
	if (file == NULL)
	{
		exit(0);
	}
	characters = fread(scanner.text, sizeof(char), scanner.bufferSize, file);

	if (characters == 0)
	{
		fout << " INVALID READ";
		exit(0);
	}

	if (characters < scanner.bufferSize)
	{
		scanner.text[characters] = '\0';
	}

	fout << "The input text is: \n\n" << scanner.text << "\n\n\n The result is: \n\n";
	while (scanner.bufferPosition < characters)
	{

		Token t = scanner.getToken();
		if (t.type != SPACE && t.type != COMMENT && t.type != INVALID)
		{
			if (t.type == STRING || t.type == KEYWORD || t.type == LETTER)
			{
				fout << "'" << t.message << "'  <-> " << token_types.find(t.type)->second << "              String table value:  " << t.value << endl;

			}
			else
			{
				fout << t.message << "  <-> " << token_types.find(t.type)->second << "              String table value: " << t.value << endl;

			}
		}
	}

	fclose(file);



}