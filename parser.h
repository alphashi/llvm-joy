#include <string>
#include <map>
#include "lexer.h"
//#include "ast.h"
#define PARSER_DEBUG true


class Parser
{

	//ExprAST* root;
	ExprAST* tail;
	std::map<int, std::string> tok_map;

	std::vector<ExprAST*> asts;
	
public:
//	ifstream ifs;
	ExprAST* root;
	Lexer* lex;
	int curTok;

	std::map<char, int> binOpPrecedence;

	Parser(Lexer* lex);
	
	int parseTok();

	int getNextToken();
	int getTokPrecedence();

	int addAST(ExprAST* ast);

	ExprAST* parseExpression();
	ExprAST* parseIdentiExpr();

	ExprAST* parseNumExpr();
	ExprAST* parseParenExpr();
	ExprAST* parsePrimary();

	ExprAST* parseBinOpRHS(int exprPrec, ExprAST *lhs);

	protoTypeAST* parseProtoType();
	functionAST* parseDefinition();
	functionAST* parseTopLevelExpr();
	protoTypeAST* parseExtern();

	int handleDefinition();
	int handleExtern();
	int handleTopLevelExpression();
	int mainLoop();


	ExprAST* Error(const char *str);
	protoTypeAST* ErrorP(const char *str);
	functionAST* ErrorF(const char *str);

};
