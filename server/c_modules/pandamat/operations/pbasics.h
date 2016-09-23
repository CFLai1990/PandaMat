#ifndef PANDAMAT_PBASICS_H
#define PANDAMAT_PBASICS_H
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

using namespace std;

enum exceptions{
	success = -1,
	typeError = 0,
	operationError = 1,
	parameterError = 2,
	noCommandError = 3,
	nanError = 4,
	unkownError = 10,
};

enum parameterType{
	p_matrix = 0,
	p_value = 1,
	p_string = 2,
	p_bool = 3,
	p_unkown = 100,
	p_missing = 1000,
};

template<typename T> string num2str(const T val){
    ostringstream oss;
    oss << val;
    return oss.str() ;
}
#endif