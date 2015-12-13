#include <iostream>
#include <vector>
#include <fstream>
#include "local_const.h"


class ExprAST
{
public:
	virtual ~ExprAST()
	{
	}

	//std::vector<ExprAST*> nexts;
	virtual int printDot(std::ofstream& ofs){return 0;}
	
	ExprAST* next;

};


class rootExprAST : public ExprAST
{
	int x;
public:
	rootExprAST();
	ExprAST* next;
	
	int printDot(std::ofstream& of);

};



class numExprAST : public ExprAST
{
	double val;

public:
	numExprAST(double _val);
//	std::vector<ExprAST*> nexts;
	ExprAST* next;
	int printDot(std::ofstream& of);

};



class varExprAST : public ExprAST
{
	std::string name;

public:	
	varExprAST(std::string &_name);
	//std::vector<ExprAST*> nexts;
	ExprAST* next;
	int printDot(std::ofstream& of);
};


class binExprAST : public ExprAST
{
	char op;
	ExprAST *lhs, *rhs;

public:
	binExprAST(char _op, ExprAST *_lhs, ExprAST *rhs);
//	std::vector<ExprAST*> nexts;
	ExprAST* next;
	int printDot(std::ofstream& of);

};




class callExprAST : public ExprAST
{
	std::string call;
	std::vector<ExprAST*> args;

public:
	ExprAST* next;
	callExprAST(const std::string &_call, std::vector<ExprAST*> &_args);
	int printDot(std::ofstream& of);
};



class protoTypeAST : public ExprAST
{
	std::string name;
	std::vector<std::string> args;

public:
	ExprAST* next;
	protoTypeAST(const std::string &_name, const std::vector<std::string> &_args);
	int printDot(std::ofstream& of);
};


class functionAST : public ExprAST
{

	protoTypeAST *proto;
	ExprAST *body;

public:
	ExprAST* next;
	functionAST(protoTypeAST *_proto, ExprAST *_body);
	int printDot(std::ofstream& of);

};



