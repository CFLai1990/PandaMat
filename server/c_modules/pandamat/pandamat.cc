#include "pandamat.h"

Persistent<Function> PandaMat::constructor;

PandaMat::PandaMat(){
	expTypes.insert(pair<string, int>("type", 0));
	stateCode = sucess;
}

void PandaMat::unpackArray(Local<Value> v_arr, parameterType& v_type, mat& v_data){
	int t_rows = 0, t_cols = 0;
	if(v_arr -> IsArray()){
		Local<Array> v_arr_i = Local<Array>::Cast(v_arr);
		t_rows = v_arr_i -> Length();
		Local<Value> v_ele_j = v_arr_i->Get(0);
		if(v_ele_j -> IsArray()){
			Local<Array> v_arr_j = Local<Array>::Cast(v_ele_j);
			t_cols = v_arr_j -> Length();
		}
	}
	if(t_rows != 0 && t_cols != 0){
		Local<Array> v_arr_i = Local<Array>::Cast(v_arr);
		v_data = mat(t_rows,t_cols);
		for(int i = 0; i < t_rows; i++){
			Local<Array> v_arr_j = Local<Array>::Cast(v_arr_i->Get(i));
			for(int j = 0; j < t_cols; j++){
				double v_element = v_arr_j -> Get(j) -> NumberValue();
				v_data(i, j) = v_element;
			}
		}
	}else{
		stateMessage = "Not a matrix!";
		throw exceptions(typeError);
	}
}

Operation* PandaMat::getOperation(const char* v_command){
	OperationResult t_result = operations.find(v_command);
	if(t_result.stateCode == sucess){
		return t_result.operation;
	}else{
		stateMessage = t_result.message;
		throw exceptions(t_result.stateCode);
	}
}

mat PandaMat::getResult(Operation* v_opr){
	OperationResult t_result = v_opr -> operate();
	if(t_result.stateCode == sucess){
		return t_result.result;
	}else{
		stateMessage = t_result.message;
		throw exceptions(t_result.stateCode);
	}
}

bool PandaMat::getState(){
	bool t_state = false;
	switch(stateCode){
		case sucess:
		t_state = true;
		stateMessage.insert(0, "Sucess!");
		break;
		case typeError:
		stateMessage.insert(0, "Type error: ");
		break;
		case operationError:
		stateMessage.insert(0, "Operation error: ");
		break;
		case parameterError:
		stateMessage.insert(0, "Parameter error: ");
		break;
		case noCommandError:
		stateMessage.insert(0, "Command error: ");
		break;
		default:
		stateMessage.insert(0, "Unknown error: ");
		break;
	}
	return t_state;
}

void PandaMat::packArray(mat& v_data){
	int t_size[2] = {(int)v_data.n_rows, (int)v_data.n_cols};
	resultMat = Array::New(isolate);
	for(int i = 0; i < t_size[0]; i++){
		Local<Array> vv_result = Array::New(isolate);
		for(int j = 0; j < t_size[1]; j++){
			vv_result->Set(j, Number::New(isolate, v_data(i,j)));
		}
		resultMat->Set(i, vv_result);
	}
}

Local<Object> PandaMat::packResult(mat& v_data){
	Local<Object> t_result = Object::New(isolate);
	bool t_state = getState();
	t_result -> Set(String::NewFromUtf8(isolate, "state"), Boolean::New(isolate, t_state));
	t_result -> Set(String::NewFromUtf8(isolate, "message"), String::NewFromUtf8(isolate, stateMessage.c_str()));
	packArray(v_data);
	t_result -> Set(String::NewFromUtf8(isolate, "result"), resultMat);
	return t_result;
}

void PandaMat::Prepare(Isolate* v_isolate){
	isolate = v_isolate;
	stateCode = sucess;
	stateMessage = "";
}

void PandaMat::Operate(const FunctionCallbackInfo<Value>& args){
	Isolate* t_isolate = args.GetIsolate();
	PandaMat* obj = ObjectWrap::Unwrap<PandaMat>(args.Holder());
	obj -> Prepare(t_isolate);
	mat t_retArr;
	try{
		if(!(args[0] -> IsString())){
			obj -> stateMessage = "Operation command missing!";
			throw parameterError;
		}
		if(!(args[1] -> IsArray())){
			obj -> stateMessage = "Variable names missing!";
			throw parameterError;
		}
		bool t_save = false;
		string t_variable;
		if(args[2] -> IsString()){
			t_save = true;
			t_variable = string(*(String::Utf8Value(args[2])));
		}
		String::Utf8Value t_command(args[0]);
		Operation* v_opr = obj -> getOperation(*t_command);
		Local<Array> t_variables = Local<Array>::Cast(args[1]);
		for(int i = 0; i < t_variables -> Length(); i++){
			String::Utf8Value t_name(t_variables -> Get(i));
			map<string, mat>::iterator it = obj -> data.find(*t_name);
			if(it == obj -> data.end()){
				obj -> stateMessage = "Variable not found!";
				throw parameterError;
			}
			v_opr -> data[i] = obj -> data.at(*t_name);
		}
		if(t_save){
			mat tt_retArr = obj -> getResult(v_opr);
			obj -> data.insert(pair<string, mat>(t_variable , tt_retArr));
		}else{
			t_retArr = obj -> getResult(v_opr);
		}
		Local<Object> result = obj -> packResult(t_retArr);
		args.GetReturnValue().Set(result);
	}
	catch(exceptions& e){
		obj -> stateCode = e;
		Local<Object> result = obj -> packResult(t_retArr);
		args.GetReturnValue().Set(result);
	}
}

void PandaMat::saveData(const FunctionCallbackInfo<Value>& args){
	Isolate* t_isolate = args.GetIsolate();
	PandaMat* obj = ObjectWrap::Unwrap<PandaMat>(args.Holder());
	obj -> Prepare(t_isolate);
	mat t_retArr;
	try{
		if(!(args[0] -> IsObject())){
			obj -> stateMessage = "Data missing!";
			throw exceptions(parameterError);
		}
		Local<Object> t_data = Local<Object>::Cast(args[0]);
		Local<Array> t_fields = t_data -> GetOwnPropertyNames();
		for(int i = 0; i < t_fields -> Length(); i++){
			Local<Value> t_field = t_fields->Get(i);
		    Local<Value> t_matrix = t_data->Get(t_field);
		    String::Utf8Value t_key(t_field);
			mat t_arr;
			parameterType t_type = p_matrix;
			obj -> unpackArray(t_matrix, t_type, t_arr);
			obj -> data.insert(pair<string, mat>(*t_key, t_arr));
		}
		Local<Object> result = obj -> packResult(t_retArr);
		args.GetReturnValue().Set(result);
	}
	catch(exceptions& e){
		obj -> stateCode = e;
		Local<Object> result = obj -> packResult(t_retArr);
		args.GetReturnValue().Set(result);
	}
}

void PandaMat::getData(const FunctionCallbackInfo<Value>& args){
	Isolate* t_isolate = args.GetIsolate();
	PandaMat* obj = ObjectWrap::Unwrap<PandaMat>(args.Holder());
	obj -> Prepare(t_isolate);
	mat t_retArr;
	try{
		if(!(args[0] -> IsString())){
			obj -> stateMessage = "Variable name missing!";
			throw parameterError;
		}
		String::Utf8Value t_variable(args[0]);
		map<string, mat>::iterator it = obj -> data.find(*t_variable);
		if(it == obj -> data.end()){
			obj -> stateMessage = "Variable not found!";
			throw operationError;
		}
		t_retArr = obj -> data.at(*t_variable);
		Local<Object> result = obj -> packResult(t_retArr);
		args.GetReturnValue().Set(result);
	}
	catch(exceptions& e){
		obj -> stateCode = e;
		Local<Object> result = obj -> packResult(t_retArr);
		args.GetReturnValue().Set(result);
	}
}

void PandaMat::New(const FunctionCallbackInfo<Value>& args){	
	Isolate* t_isolate = args.GetIsolate();
	if (args.IsConstructCall()) {
    // Invoked as constructor: `new MyObject(...)`
		PandaMat* obj = new PandaMat();
		obj->Wrap(args.This());
		args.GetReturnValue().Set(args.This());
	} else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
		const int argc = 1;
		Local<Value> argv[argc] = { args[0] };
		Local<Context> context = t_isolate->GetCurrentContext();
		Local<Function> cons = Local<Function>::New(t_isolate, constructor);
		Local<Object> result =
		cons->NewInstance(context, argc, argv).ToLocalChecked();
		args.GetReturnValue().Set(result);
	}
}

void PandaMat::Init(Isolate* v_isolate){
  // Prepare constructor template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(v_isolate, New);
	tpl->SetClassName(String::NewFromUtf8(v_isolate, "PandaMat"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "get", getData);
	NODE_SET_PROTOTYPE_METHOD(tpl, "save", saveData);
	NODE_SET_PROTOTYPE_METHOD(tpl, "operate", Operate);
	constructor.Reset(v_isolate, tpl->GetFunction());
}

void PandaMat::NewInstance(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  const unsigned argc = 1;
  Local<Value> argv[argc] = { args[0] };
  Local<Function> cons = Local<Function>::New(isolate, constructor);
  Local<Context> context = isolate->GetCurrentContext();
  Local<Object> instance =
      cons->NewInstance(context, argc, argv).ToLocalChecked();

  args.GetReturnValue().Set(instance);
}