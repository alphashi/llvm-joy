#include <iostream>
//#include <fstream>
//#include <cstdlib>

#include "local_const.h"
//#include "lexer.h"
#include "parser.h"
#include "macro.h"



using namespace std;


int readFile(string &fname)
{

	string line;

	std::ifstream file(fname.c_str());

	if (!file.good())
	{
		cout << "open file FAILED." << endl;
	}
	
	
	string content = "";

	int lnum = 0;

	//char character;

	Lexer* lex = new Lexer(&file);
	
	Parser* parse = new Parser(lex);

	parse->mainLoop();


//	cout << "dump file end" << endl;

	ofstream ofs("ast.dot");

	if (!ofs.good())
	{
		cout << "open file error" << endl;
		return RET_FAILURE;
	}


//	ofs << "digraph G \n {" << endl;
	
	

//	ExprAST* node = parse->root;

//	while(node != NULL)
//	{
//		node->printDot(ofs);
//		node = node->next;
//	}
//
//	ofs << "}" << endl;
	
	ofs.close();


//	cout << "\ntest_case_0" << endl;

	return RET_SUCCESS;
}




int main()
{


	cout << "cout test." << endl;
	//string fname = "hello.txt";
	string fname = "code.mat";


	INF_LOG(true,"The file name is: %s .\n", fname.c_str());


	if(readFile(fname) == RET_SUCCESS)
			cout << "return success" << endl;;


//	system("pause");


	return RET_SUCCESS;
}






