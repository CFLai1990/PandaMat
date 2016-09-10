#ifndef PANDAMAT_OPERATION_H
#define PANDAMAT_OPERATION_H
#include <iostream>
#include <stdlib.h>
#include <string>
#include <node.h>
#include <v8.h>
#include <armadillo>
  
using namespace std;
using namespace node;
using namespace v8;
using namespace arma;

enum exceptions{
	sucess = -1,
	typeError = 0,
	operationError = 1,
	parameterError = 2,
	noCommandError = 3,
	unkownError = 10,
};

enum parameterType{
	p_matrix = 0,
	p_value = 1,
	p_string = 2,
};

class Operation;

class OperationResult{
public: 
	OperationResult(){
		stateCode = sucess;
	};
	exceptions stateCode;
	string message;
	mat result;
	Operation* operation;
};

class matSize{
public:
	int cols;
	int rows;
};

class Operation{
public: 
	Operation(){};
	Operation(int v_par, parameterType* v_types);
	int parameterNum;
	enum parameterType parameterTypes[10];
	mat data[2];
	virtual OperationResult operate();
protected:
	matSize getSize(mat v_data);
};
#endif
