#ifndef PANDAMAT_H
#define PANDAMAT_H
#define ARMA_DONT_USE_WRAPPER
#include <iostream>  
#include <sstream>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <map>
#include <string>
#include <node.h>
#include <node_object_wrap.h>
#include <v8.h>
#include <armadillo>
#include <boost/filesystem.hpp>
#include "pbasics.h"
#include "operation.h"
#include "opercollection.h"
#include "pandapool.h"

using namespace std;
using namespace node;
using namespace v8;
using namespace arma;
using namespace boost::filesystem;

const string userDataPath = "/var/www/html/PandaMat/server/c_modules/pandamat/usrdata/";

class PandaMat: public node::ObjectWrap{
public:
	PandaMat();
	~PandaMat();
	static void Init(Isolate* v_isolate);
	static void NewInstance(const FunctionCallbackInfo<Value>& args);
  	static Persistent<Function> constructor;
protected:
	void registerUser(string v_pandaID);
	unsigned int enter(Isolate* v_isolate);
	void exit(unsigned int v_id);
	void setState(unsigned int v_id, exceptions v_code, string v_message);
	void save(unsigned int v_id, Local<Value> v_data, const char* v_name);
	void saveResult(unsigned int v_id, const char * v_to, const char * v_from);
	void saveResult(unsigned int v_id, const char * v_name);
	void check(unsigned int v_id, const char* v_name);
	void get(unsigned int v_id, const char* v_name);
	void remove(unsigned int v_id, const char* v_name);
	void send(unsigned int v_id, Operation* v_opr, const char* v_name, unsigned int v_i);
	void getResult(unsigned int v_id, Operation* v_opr);
	Operation* getOperation(unsigned int v_id, const char* v_command);
	Local<Object> packResult(unsigned int v_id, bool v_result);
  	string pandaID;
  	string pandaPath;
private:
	static void New(const FunctionCallbackInfo<Value>& args);
	static void Operate(const FunctionCallbackInfo<Value>& args);
	static void saveData(const FunctionCallbackInfo<Value>& args);
	static void removeData(const FunctionCallbackInfo<Value>& args);
	static void getData(const FunctionCallbackInfo<Value>& args);
	static void exitUser(const FunctionCallbackInfo<Value>& args);
	OperationCollection operations;
	PandaPool pool;
};
#endif
