#include "local_const.h"
#include "parser.h"
#include "macro.h"


Parser::Parser(Lexer* _lex)
: lex(_lex), curTok(0)
{
	this->root = new rootExprAST();
	this->tail = this->root;
	this->asts.push_back(this->root);

	// initial token map
	tok_map.insert(std::make_pair<int, std::string>(tok_eof, "EOF"));
	tok_map.insert(std::make_pair<int, std::string>(tok_def, "DEFINE"));
	tok_map.insert(std::make_pair<int, std::string>(tok_extern, "EXTERN"));
	tok_map.insert(std::make_pair<int, std::string>(tok_identifier, "IDENTIFIER"));
	tok_map.insert(std::make_pair<int, std::string>(tok_number, "NUMBER"));
	tok_map.insert(std::make_pair<int, std::string>(tok_char, "char"));


	binOpPrecedence.insert(std::make_pair<char, int>('+', 10));
	binOpPrecedence.insert(std::make_pair<char, int>('-', 20));
	binOpPrecedence.insert(std::make_pair<char, int>('*', 30));
	binOpPrecedence.insert(std::make_pair<char, int>('/', 40));

	INF_LOG(PARSER_DEBUG, "--> add root to AST.\n");
};



int Parser::getNextToken()
{
	this->curTok = lex->getTok();

	INF_LOG(PARSER_DEBUG, "current token is: %s .\n",
			tok_map[MIN(0, this->curTok)].c_str());

	return this->curTok;
};



int Parser::getTokPrecedence()
{
	if (!isascii(curTok))
	{
		return RET_FAILURE;
	}
	
	// make sure it's a declared binOp
	int tokPrec = binOpPrecedence[curTok];
	
	if (tokPrec <= 0)
	{
		return RET_FAILURE;
	}

	return tokPrec;
};


int Parser::addAST(ExprAST* ast)
{
	if (ast == NULL)
	{
		return RET_FAILURE;
	}

	this->tail->next = ast;
	this->tail = ast;
	this->asts.push_back(ast);

	return RET_SUCCESS;
};



ExprAST* Parser::parseExpression()
{
	ExprAST* lhs = this->parsePrimary();

	if (!lhs)
	{
		return RET_PTR_FAILURE;
	}
	
	return this->parseBinOpRHS(0, lhs);
};



ExprAST* Parser::parseIdentiExpr()
{
	std::string idName = lex->identiStr;

	this->getNextToken();

	if (curTok != '(')
	{
		varExprAST* varExpr = new varExprAST(idName);
		this->addAST(varExpr);
		INF_LOG(PARSER_DEBUG, "--> add varExpr to AST.\n");

		return varExpr;
	}

	this->getNextToken();

	std::vector<ExprAST*> args;

	if (curTok != ')')
	{
		while(1)
		{
			ExprAST *arg = this->parseExpression();

			if (!arg)
			{
				return RET_PTR_FAILURE;
			}

			args.push_back(arg);

			if (curTok != ',')
			{
				return Error("Expected ')' or ',' in argument list");
			}

			this->getNextToken();
		}
	}
};



ExprAST* Parser::parseNumExpr()
{
	ExprAST *ret = new numExprAST(lex->numVal);
	this->addAST(ret);
	INF_LOG(PARSER_DEBUG, "--> add numExpr to AST.\n");

	this->getNextToken();
	
	return ret;
};


ExprAST* Parser::parseParenExpr()
{
	this->getNextToken();
	ExprAST *ret = this->parseExpression();

	if (!ret)
	{
		return RET_PTR_FAILURE;
	}

	if (curTok != ')')
	{
		return Error("expected ')'");
	}

	this->getNextToken();

	return ret;
};



ExprAST* Parser::parsePrimary()
{
	switch (this->curTok)
	{
		case tok_identifier:
			return this->parseIdentiExpr();

		case tok_number:
			return this->parseNumExpr();

		case '(':
			return this->parseParenExpr();
		
		default:
			return Error ("unknown token when expecting an expression");
	}
};


/// binoprhs
///		::= ('+' primary)*
ExprAST* Parser::parseBinOpRHS(int exprPrec, ExprAST* lhs)
{
	
	while(1)
	{
		int tokPrec = this->getTokPrecedence();

		if (tokPrec < exprPrec)
		{
			return lhs;
		}

		int binOp = curTok;
		this->getNextToken();

		ExprAST* rhs = this->parsePrimary();

		if (!rhs)
		{
			return RET_PTR_FAILURE;
		}

		int nextPrec = this->getTokPrecedence();

		if (tokPrec < nextPrec)
		{
			rhs = this->parseBinOpRHS(tokPrec+1, rhs);

			if (rhs == NULL)
			{
				return RET_PTR_FAILURE;
			}
		}

		// Merge lhs/rhs
		lhs = new binExprAST(binOp, lhs, rhs);
	}
};


/// prototype
///		::= id '(' id* ')'
protoTypeAST* Parser::parseProtoType()
{
	if (curTok != tok_identifier)
	{
		return ErrorP("Expected function name in prototype");
	}

	std::string fnName = lex->identiStr;
	INF_LOG(PARSER_DEBUG, "get function name is: %s\n", fnName.c_str());

	this->getNextToken();

	if (curTok != '(')
	{
		return ErrorP("Expected '(' in prototype");
	}

	// parse all the arguments
	std::vector<std::string> argNames;

	while (this->getNextToken() == tok_identifier)
	{
		argNames.push_back(lex->identiStr);
		if (this->getNextToken() != ',')
		{
			break;
		}
	}

	INF_LOG(PARSER_DEBUG, "get arguments, size is: %d\n", argNames.size());


	if (curTok != ')')
	{
		return ErrorP("Expected ')' in prototype");
	}

	this->getNextToken();

	protoTypeAST* ret_ast = new protoTypeAST(fnName, argNames);

	this->addAST(ret_ast);
	INF_LOG(PARSER_DEBUG, "--> add protoType to AST.\n");

//	this->tail->next = ret_ast;
//	this->tail = ret_ast;
//	this->asts.push_back(ret_ast);

	return ret_ast;

};


functionAST* Parser::parseDefinition()
{
	this->getNextToken();
	protoTypeAST* proto = this->parseProtoType();

	if (proto == NULL)
	{
		return RET_PTR_FAILURE;
	}

	if (ExprAST* e = this->parseExpression())
	{
		functionAST* fun = new functionAST(proto, e);

		this->addAST(fun);
		INF_LOG(PARSER_DEBUG, "--> add functionAST to AST.\n");

//		this->tail->next = fun;
//		this->tail = fun;
//		this->asts.push_back(fun);

		return fun;
	}
	else
	{
		return RET_PTR_FAILURE;
	}
};


functionAST* Parser::parseTopLevelExpr()
{
	if (ExprAST* e = this->parseExpression())
	{
		protoTypeAST* proto = new protoTypeAST("__anon_expr", std::vector<std::string>());

		functionAST* fun = new functionAST(proto, e);

		this->addAST(fun);
		INF_LOG(PARSER_DEBUG, "--> add functionAST to AST.\n");

//		this->tail->next = fun;
//		this->tail = fun;
//		this->asts.push_back(fun);

		return fun;
	}
	else
	{
		return RET_PTR_FAILURE;
	}
};



protoTypeAST* Parser::parseExtern()
{
	this->getNextToken();

	return this->parseProtoType();

};



int Parser::handleDefinition()
{
	if (this->parseDefinition())
	{
		INF_LOG(PARSER_DEBUG, "Parsed a function definition.\n");
	}
	else
	{
		this->getNextToken();
	}

	return RET_SUCCESS;
};


int Parser::handleExtern()
{
	if (this->parseExtern())
	{
		INF_LOG(PARSER_DEBUG, "Parsed an extern.\n");
		return RET_SUCCESS;
	}
	else
	{
		this->getNextToken();
		return RET_FAILURE;
	}

};


int Parser::handleTopLevelExpression()
{
	if (this->parseTopLevelExpr())
	{
		INF_LOG(PARSER_DEBUG, "Parsed a top-level expr.\n");
		return RET_SUCCESS;
	}
	else
	{
		this->getNextToken();
		return RET_FAILURE;
	}
};


int Parser::mainLoop()
{
	while (1)
	{
		INF_LOG(PARSER_DEBUG, "ready to parse.\n");

		this->getNextToken();

		switch (this->curTok)
		{
		case tok_eof:
			return RET_SUCCESS;

		case ';':
			this->getNextToken();
			break;

		case tok_def:
			this->handleDefinition();
			break;

		case tok_extern:
			this->handleExtern();
			break;

		default:
			handleTopLevelExpression();
			break;
		}
	}

	return RET_SUCCESS;
}



ExprAST* Parser :: Error(const char *str)
{
	fprintf(stderr, "Error: %s\n", str);
	return RET_SUCCESS;
};

protoTypeAST* Parser :: ErrorP(const char *str)
{
	this->Error(str);
	return RET_SUCCESS;

};

functionAST* Parser :: ErrorF(const char *str)
{
	this->Error(str);
	return RET_SUCCESS;

};



