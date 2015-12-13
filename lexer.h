#include <map>
#include <iostream>
#include <fstream>
#include "ast.h"

enum Token 
{
	tok_eof = -1,

	tok_def = -2,
	tok_extern = -3,

	tok_identifier = -4,
	tok_number = -5,

	tok_char = 0
};



class Lexer
{

//	ExprAST* root;
//	std::vector<ExprAST*> asts;

public:

	std::ifstream *ifs;
	int lastChar;
	std::string identiStr;
	double numVal;

	Lexer(std::ifstream *_ifs);

	int skipWhitespace();
	int getString();
	int getDigital();
	int skipComment();

	int getTok();


};
