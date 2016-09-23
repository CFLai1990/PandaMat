#ifndef PANDAMAT_OPERATION_H
#define PANDAMAT_OPERATION_H
#include <iostream>
#include <stdlib.h>
#include <string>
#include <node.h>
#include <v8.h>
#include <map>
#include <string>
#include <armadillo>
#include "pbasics.h"
#include "operationdata.h"
  
using namespace std;
using namespace node;
using namespace v8;
using namespace arma;

class Operation;

class OperationResult{
public: 
	OperationResult(){
		stateCode = success;
	};
	exceptions stateCode;
	string stateMessage;
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
	Operation(int v_par, parameterType* v_types, parameterType v_type);
	int parameterNum;
	enum parameterType parameterTypes[10];
	OperationData data;
	virtual void operate();
protected:
	matSize getSize(mat v_data);
	void setResult(exceptions v_code, string v_message);
};
#endif
