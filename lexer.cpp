#include <cstdlib>
#include "macro.h"
//#include "ast.h"
#include "lexer.h"
//#include "parser.h"
#include "local_const.h"
#include <string>


#define LEX_DEBUG true

Lexer::Lexer(std::ifstream *_ifs)
: ifs(_ifs)
{
	lastChar = ' ';
	identiStr = ' ';
	numVal = 0;

}



int Lexer::skipWhitespace()
{
	while(isspace(lastChar))
	{
		lastChar = this->ifs->get(); 
	}

	return RET_SUCCESS;
}



int Lexer::getString()
{
	identiStr = lastChar;

	while (isalnum((lastChar = this->ifs->get())))
	{
		identiStr += lastChar;
	}

	INF_LOG(LEX_DEBUG, "Get string is: %s \n", identiStr.c_str());

	if (identiStr == "def")
	{
		INF_LOG(LEX_DEBUG, "Define found. \n");
		return tok_def;
	}

	if (identiStr == "extern")
	{
		INF_LOG(LEX_DEBUG, "Extern found. \n");
		return tok_extern;
	}

	return tok_identifier;

}



int Lexer::getDigital()
{
	std::string numStr;
	do
	{
		numStr += lastChar;
		lastChar = this->ifs->get();
	}
	while (isdigit(lastChar) || lastChar == '.');

	INF_LOG(LEX_DEBUG, "Number found.\n");

	numVal = strtod(numStr.c_str(), 0);
	
	INF_LOG(LEX_DEBUG, "the number is: %f \n", numVal);

	return tok_number;

}



int Lexer::skipComment()
{
	INF_LOG(LEX_DEBUG, "Skip comment.\n");

	do
	{
		lastChar = this->ifs->get();
	}
	while (lastChar != EOF && lastChar != '\n' && lastChar != '\r');

	if (lastChar != EOF)
	{
		return this->getTok();
	}

	return RET_FAILURE;

}



int Lexer::getTok()
{
	INF_LOG(LEX_DEBUG, "===> Get token.\n");

//	lastChar = this->ifs->get();

	INF_LOG(LEX_DEBUG, "current char is: %c; ACISS is: %3d\n", lastChar, lastChar);

	this->skipWhitespace();

	INF_LOG(LEX_DEBUG, "current char is: %c; ACISS is: %3d\n", lastChar, lastChar);

	if (isalpha(lastChar))
	{
		return this->getString();
	}
	else if (isdigit(lastChar) || lastChar == '.')
	{
		return this->getDigital();
	}
	else if (lastChar == '#')
	{
		return this->skipComment();
	}
	else if (lastChar == EOF)
	{
		INF_LOG(LEX_DEBUG, "Get EOF.\n");
		return tok_eof;
	}
	
	int thisChar = lastChar;
	lastChar = this->ifs->get();

	INF_LOG(LEX_DEBUG, "<=== END Get token.\n");

	return thisChar;

}


