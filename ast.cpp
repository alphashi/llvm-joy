
#include "ast.h"

rootExprAST :: rootExprAST()
: x(0), next(NULL) 
{ 
}; 

int rootExprAST :: printDot(std::ofstream& ofs)
{
	ofs << "Root";

	return RET_SUCCESS;
};



numExprAST :: numExprAST(double _val)
: val(_val), next(NULL) 
{ 
}; 


int numExprAST :: printDot(std::ofstream& ofs)
{
	ofs << "->numExprAST";
	return RET_SUCCESS;

};


varExprAST :: varExprAST(std::string &_name)
: name(_name), next(NULL)
{
};


int varExprAST :: printDot(std::ofstream& ofs)
{
	ofs << "->varExprAST";
	return RET_SUCCESS;

}

binExprAST :: binExprAST(char _op, ExprAST *_lhs, ExprAST *_rhs)
: op(_op), lhs(_lhs), rhs(_rhs), next(NULL)
{
};


int binExprAST :: printDot(std::ofstream& ofs)
{
	ofs << "->binExprAST";
	return RET_SUCCESS;

}


callExprAST :: callExprAST(const std::string &_call, std::vector<ExprAST*> &_args)
: call(_call), args(_args), next(NULL)
{
};


int callExprAST :: printDot(std::ofstream& ofs)
{
	ofs << "->callExprAST";
	return RET_SUCCESS;

}

protoTypeAST :: protoTypeAST(const std::string &_name, const std::vector<std::string> &_args)
: name(_name), args(_args), next(NULL)
{
};

int protoTypeAST :: printDot(std::ofstream& ofs)
{
	ofs << "protoTypeAST";
	return RET_SUCCESS;

}


functionAST :: functionAST(protoTypeAST *_proto, ExprAST *_body)
: proto(_proto), body(_body), next(NULL)
{
};

int functionAST :: printDot(std::ofstream& ofs)
{
	ofs << "functionAST";
	return RET_SUCCESS;

}
