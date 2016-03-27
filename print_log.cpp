/*
 * print_log.cpp
 *
 *  Created on: Jan 23, 2016
 *      Author: alpha
 */


#include <cstdlib>
#include <cstdarg>

#include <iostream>

#include "local_const.h"
#include "print_log.h"

//#define LOG_PRINT true
#define LOG_LEN 200

const char *log_fname = "debug.log";
FILE *file = fopen(log_fname, "w+");


void logprint(const char *pFmt, ...)
{
//	std::cout << "... log print ...." << std::endl;
	char log_info[LOG_LEN];
	va_list pArgList;

	va_start(pArgList, pFmt);
	vsnprintf(log_info, LOG_LEN, pFmt, pArgList);
	va_end(pArgList);

	if (LOG_PRINT)
	{
//		std::cout << "in print ...." << std::endl;
		fprintf(file, "%s", log_info);
	}
	fflush(file);

};


