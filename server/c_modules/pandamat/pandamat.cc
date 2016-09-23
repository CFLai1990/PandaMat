#include "pandamat.h"

Persistent<Function> PandaMat::constructor;

PandaMat::PandaMat(){

}

unsigned int PandaMat::enter(Isolate* v_isolate){
	unsigned int t_id = pool.enter(v_isolate);
	return t_id;
}

void PandaMat::exit(unsigned int v_id){
	pool.exit(v_id);
}

void PandaMat::setState(unsigned int v_id, exceptions v_code, string v_message){
	pool.setState(v_id, v_code, v_message);
}

void PandaMat::save(unsigned int v_id, Local<Value> v_data, const char* v_name){
	exceptions t_state = pool.save(v_id, v_data, v_name);
	if(t_state != success){
		throw exceptions(t_state);
	}
}

void PandaMat::saveResult(unsigned int v_id, const char * v_to, const char * v_from){
	exceptions t_state = pool.saveResult(v_id, v_to, v_from);
	if(t_state != success){
		throw exceptions(t_state);
	}
}

void PandaMat::saveResult(unsigned int v_id, const char * v_name){
	exceptions t_state = pool.saveResult(v_id, v_name);
	if(t_state != success){
		throw exceptions(t_state);
	}
}

void PandaMat::check(unsigned int v_id, const char* v_name){
	exceptions t_state = pool.check(v_id, v_name);
	if(t_state != success){
		throw exceptions(t_state);
	}
}

void PandaMat::get(unsigned int v_id, const char* v_name){
	exceptions t_state = pool.get(v_id, v_name);
	if(t_state != success){
		throw exceptions(t_state);
	}
}

void PandaMat::remove(unsigned int v_id, const char* v_name){
	exceptions t_state = pool.remove(v_id, v_name);
	if(t_state != success){
		throw exceptions(t_state);
	}
}

void PandaMat::send(unsigned int v_id, Operation* v_opr, const char* v_name, unsigned int v_i){
	exceptions t_state = pool.send(v_id, v_opr -> data, v_opr -> parameterTypes[v_i], v_name, num2str(v_i).c_str());
	if(t_state != success){
		throw exceptions(t_state);
	}
}


void PandaMat::getResult(unsigned int v_id, Operation* v_opr){
	exceptions t_state = pool.getResult(v_id, v_opr -> data);
	operations.destroy(v_id);
	if(t_state != success){
		throw exceptions(t_state);
	}
}

Operation* PandaMat::getOperation(unsigned int v_id, const char* v_command){
	OperationResult t_result = operations.create(v_id, v_command);
	if(t_result.stateCode == success){
		return t_result.operation;
	}else{
		setState(v_id, t_result.stateCode, t_result.stateMessage);
		throw exceptions(t_result.stateCode);
	}
}

Local<Object> PandaMat::packResult(unsigned int v_id, bool v_result){
	Isolate* t_isolate = pool.getIsolate(v_id);
	bool t_state = pool.packState(v_id);
	string t_message = pool.packMessage(v_id);
	parameterType t_type = pool.getType(v_id);
	Local<Object> t_result = Object::New(t_isolate);
	t_result -> Set(String::NewFromUtf8(t_isolate, "state"), Boolean::New(t_isolate, t_state));
	t_result -> Set(String::NewFromUtf8(t_isolate, "message"), String::NewFromUtf8(t_isolate, t_message.c_str()));
	if(v_result){
		switch(t_type){
			case p_matrix:
				t_result -> Set(String::NewFromUtf8(t_isolate, "result"), pool.getResultMat(v_id));
			break;
			case p_value:
				t_result -> Set(String::NewFromUtf8(t_isolate, "result"), pool.getResultValue(v_id));
			break;
			case p_string:
				t_result -> Set(String::NewFromUtf8(t_isolate, "result"), pool.getResultString(v_id));
			break;
			case p_bool:
				t_result -> Set(String::NewFromUtf8(t_isolate, "result"), pool.getResultBool(v_id));
			break;
			case p_missing:
				t_result -> Set(String::NewFromUtf8(t_isolate, "result"), Null(t_isolate));
			break;
		}
	}else{
		t_result -> Set(String::NewFromUtf8(t_isolate, "result"), Null(t_isolate));
	}
	return t_result;
}

void PandaMat::Operate(const FunctionCallbackInfo<Value>& args){
	Isolate* t_isolate = args.GetIsolate();
	PandaMat* obj = ObjectWrap::Unwrap<PandaMat>(args.Holder());
	unsigned int t_id = obj -> enter(t_isolate);
	try{
		if(!(args[0] -> IsString())){
			obj -> setState(t_id, parameterError, "Operation command missing!");
			throw exceptions(parameterError);
		}
		if(!(args[1] -> IsArray())){
			obj -> setState(t_id, parameterError, "Variable names missing!");
			throw exceptions(parameterError);
		}
		bool t_save = false;
		string t_saveVariable;
		bool t_return = false;
		if(args[2] -> IsString()){
			t_save = true;
			t_saveVariable = string(*(String::Utf8Value(args[2])));
		}
		if(args[3] -> IsBoolean()){
			t_return = Local<Boolean>::Cast(args[3]) -> BooleanValue();
		}
		String::Utf8Value t_command(args[0]);
		Local<Array> t_variables = Local<Array>::Cast(args[1]);
		parameterType tt_type = p_missing;
		if(string(*t_command).compare("equal") == 0){
			String::Utf8Value t_left(t_variables -> Get(0));
			String::Utf8Value t_right(t_variables -> Get(1));
			obj -> check(t_id, *t_right);
			obj -> saveResult(t_id, *t_left, *t_right);
		}else{
			Operation* t_opr = obj -> getOperation(t_id, *t_command);
			for(unsigned int i = 0; i < t_variables -> Length(); i++){
				String::Utf8Value t_name(t_variables -> Get(i));
				obj -> check(t_id, *t_name);
				obj -> send(t_id, t_opr, *t_name, i);
			}
			t_opr -> operate();
			obj -> getResult(t_id, t_opr);
			if(t_save){
				obj -> saveResult(t_id, t_saveVariable.c_str());
			}
		}
		Local<Object> result = obj -> packResult(t_id, t_return);
		obj -> exit(t_id);
		args.GetReturnValue().Set(result);
	}
	catch(exceptions&){
		Local<Object> result = obj -> packResult(t_id, false);
		obj -> exit(t_id);
		args.GetReturnValue().Set(result);
	}
}

void PandaMat::saveData(const FunctionCallbackInfo<Value>& args){
	Isolate* t_isolate = args.GetIsolate();
	PandaMat* obj = ObjectWrap::Unwrap<PandaMat>(args.Holder());
	unsigned int t_id = obj -> enter(t_isolate);
	try{
		if(!(args[0] -> IsObject())){
			obj -> setState(t_id, parameterError, "Data missing!");
			throw exceptions(parameterError);
		}
		Local<Object> t_data = Local<Object>::Cast(args[0]);
		Local<Array> t_fields = t_data -> GetOwnPropertyNames();
		for(unsigned int i = 0; i < t_fields -> Length(); i++){
			Local<Value> t_field = t_fields->Get(i);
		    Local<Value> tt_data = t_data->Get(t_field);		    
		    String::Utf8Value t_key(t_field);
		    obj -> save(t_id, tt_data, *t_key);
		}
		Local<Object> result = obj -> packResult(t_id, false);
		obj -> exit(t_id);
		args.GetReturnValue().Set(result);
	}
	catch(exceptions&){
		Local<Object> result = obj -> packResult(t_id, false);
		obj -> exit(t_id);
		args.GetReturnValue().Set(result);
	}
}

void PandaMat::removeData(const FunctionCallbackInfo<Value>& args){
	Isolate* t_isolate = args.GetIsolate();
	PandaMat* obj = ObjectWrap::Unwrap<PandaMat>(args.Holder());
	unsigned int t_id = obj -> enter(t_isolate);
	try{
		if(args[0] -> IsArray()){
			Local<Array> t_variables = Local<Array>::Cast(args[0]);
			for(unsigned int i = 0; i < t_variables -> Length(); i++){
				Local<Value> t_variable = t_variables->Get(i);
			    String::Utf8Value t_key(t_variable);
			    obj -> remove(t_id, *t_key);
			}
		}
		Local<Object> result = obj -> packResult(t_id, false);
		obj -> exit(t_id);
		args.GetReturnValue().Set(result);
	}
	catch(exceptions&){
		Local<Object> result = obj -> packResult(t_id, false);
		obj -> exit(t_id);
		args.GetReturnValue().Set(result);
	}
}

void PandaMat::getData(const FunctionCallbackInfo<Value>& args){
	Isolate* t_isolate = args.GetIsolate();
	PandaMat* obj = ObjectWrap::Unwrap<PandaMat>(args.Holder());
	unsigned int t_id = obj -> enter(t_isolate);
	try{
		if(!(args[0] -> IsString())){
			obj -> setState(t_id, parameterError, "Variable name missing!");
			throw exceptions(parameterError);
		}
		String::Utf8Value t_variable(args[0]);
		obj -> get(t_id, *t_variable);
		Local<Object> result = obj -> packResult(t_id, true);
		obj -> exit(t_id);
		args.GetReturnValue().Set(result);
	}
	catch(exceptions&){
		Local<Object> result = obj -> packResult(t_id, false);
		obj -> exit(t_id);
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
	NODE_SET_PROTOTYPE_METHOD(tpl, "remove", removeData);
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