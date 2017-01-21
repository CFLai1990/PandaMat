#ifndef PANDAMAT_OPERATIONDATA_H
#define PANDAMAT_OPERATIONDATA_H
#include <iostream>
#include <stdlib.h>
#include <string>
#include <node.h>
#include <v8.h>
#include <map>
#include <string>
#include <armadillo>
#include "pbasics.h"
  
using namespace std;
using namespace node;
using namespace v8;
using namespace arma;

class OperationData{
public: 
	OperationData();
	string stateMessage;
	exceptions stateCode;
	parameterType resultType;
	vec resultVec;
	mat resultMat;
	double resultValue;
	string resultString;
	bool resultBool;
	void setState(exceptions v_code, const char * v_message);
	parameterType find(const char* v_variable);
	parameterType find(unsigned int v_ord);
	bool getData(unsigned int v_ord, vec& v_vec);
	bool getData(unsigned int v_ord, mat& v_mat);
	bool getData(unsigned int v_ord, double& v_value);
	bool getData(unsigned int v_ord, string& v_string);
	bool getData(unsigned int v_ord, bool& v_bool);
	void saveData(vec v_vec, const char* v_name);
	void saveData(mat v_mat, const char* v_name);
	void saveData(double v_value, const char* v_name);
	void saveData(string v_string, const char* v_name);
	void saveData(bool v_bool, const char* v_name);
	void setResultType(parameterType v_type);
	void saveResultV(const vec& v_vec);
	void saveResult(const mat& v_mat);
	void saveResult(const double& v_value);
	void saveResult(const string& v_string);
	void saveResult(const bool& v_bool);
protected:
	map<string, vec> dataVec;
	map<string, mat> dataMat;
	map<string, double> dataValue;
	map<string, string> dataString;
	map<string, bool> dataBool;
	map<string, parameterType> dataPool;
};
#endif
