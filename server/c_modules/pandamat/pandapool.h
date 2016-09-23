#ifndef PANDAMAT_PANDAPOOL_H
#define PANDAMAT_PANDAPOOL_H
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

class PandaPool: public OperationData{
public:
	PandaPool();
	//Enter and exit
	unsigned int enter(Isolate* v_isolate);
	void exit(unsigned int v_id);
	Isolate* getIsolate(unsigned int v_id);
	//State functions
	void setState(unsigned int v_id, exceptions v_code, string v_message);
	bool packState(unsigned int v_id);
	string packMessage(unsigned int v_id);
	//Data functions
	exceptions save(unsigned int v_id, Local<Value> v_data, const char* v_name);
	exceptions saveResult(unsigned int v_id, const char* v_to, const char* v_from);
	exceptions saveResult(unsigned int v_id, const char* v_name);
	exceptions check(unsigned int v_id, const char* v_name);
	exceptions get(unsigned int v_id, const char* v_name);
	exceptions getResult(unsigned int v_id, OperationData& v_data);
	exceptions remove(unsigned int v_id, const char* v_name);
	exceptions send(unsigned int v_id, OperationData& v_oprData, parameterType v_type, const char* v_from, const char* v_to);
	parameterType getType(Local<Value> v_data);
	parameterType getType(unsigned int v_id);
	Local<Array> getResultMat(unsigned int v_id);
	Local<Number> getResultValue(unsigned int v_id);
	Local<String> getResultString(unsigned int v_id);
	Local<Boolean> getResultBool(unsigned int v_id);
private:
	unsigned int currentID;
	map<unsigned int, Isolate*> isolatePool;
	map<unsigned int, exceptions> statePool;
	map<unsigned int, string> messagePool;
	map<unsigned int, mat> matPool;
	map<unsigned int, double> numberPool;
	map<unsigned int, string> stringPool;
	map<unsigned int, bool> boolPool;
	map<unsigned int, parameterType> typePool;
	mat getMat(unsigned int v_id);
	double getValue(unsigned int v_id);
	string getString(unsigned int v_id);
	bool getBool(unsigned int v_id);
	void unpackArray(unsigned int v_id, Local<Value> v_mat, mat& v_data);
	void packArray(unsigned int v_id, mat v_mat, Local<Array>& v_arr);
};
#endif
