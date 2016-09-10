#ifndef PANDAMAT_H
#define PANDAMAT_H
#include <iostream>  
#include <stdlib.h>
#include <map>
#include <string>
#include <node.h>
#include <node_object_wrap.h>
#include <v8.h>
#include <armadillo>
#include "operation.h"
#include "opercollection.h"
  
using namespace std;
using namespace node;
using namespace v8;
using namespace arma;

class PandaMat: public node::ObjectWrap{
public:
	PandaMat();
	static void Init(Isolate* v_isolate);
	static void NewInstance(const FunctionCallbackInfo<Value>& args);
  	static Persistent<Function> constructor;
protected:
	void Prepare(Isolate* v_isolate);
	void unpackArray(Local<Value> v_arr, parameterType& v_type, mat& v_data);
	void packArray(mat& v_data);
	Operation* getOperation(const char* v_command);
	bool getState();
	mat getResult(Operation* v_opr);
	Local<Object> packResult(mat& v_data);
private:
	static void New(const FunctionCallbackInfo<Value>& args);
	static void saveData(const FunctionCallbackInfo<Value>& args);
	static void getData(const FunctionCallbackInfo<Value>& args);
	static void Operate(const FunctionCallbackInfo<Value>& args);
	OperationCollection operations;
	map<string, mat> data;
	Isolate* isolate;
	map<string, int> expTypes;
	string stateMessage;
	exceptions stateCode;
	Local<Array> resultMat;
};
#endif
