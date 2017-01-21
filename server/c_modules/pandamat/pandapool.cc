	#include "pandapool.h"

PandaPool::PandaPool(){
	currentID = 0;
}

//Enter and exit
unsigned int PandaPool::enter(Isolate* v_isolate){
	if(currentID >= 65535){
		currentID = 0;
	}
	unsigned int t_id = currentID;
	currentID ++;
	isolatePool.insert(pair<unsigned int, Isolate*>(t_id, v_isolate));
	statePool.insert(pair<unsigned int, exceptions>(t_id, success));
	messagePool.insert(pair<unsigned int, string>(t_id, "Success!"));
	typePool.insert(pair<unsigned int, parameterType>(t_id, p_missing));
	return t_id;
};

void PandaPool::exit(unsigned int v_id){
	isolatePool.erase(v_id);
	statePool.erase(v_id);
	messagePool.erase(v_id);
	vecPool.erase(v_id);
	matPool.erase(v_id);
	numberPool.erase(v_id);
	stringPool.erase(v_id);
	boolPool.erase(v_id);
	typePool.erase(v_id);
};

Isolate* PandaPool::getIsolate(unsigned int v_id){
	return isolatePool.at(v_id);
};

//State functions
void PandaPool::setState(unsigned int v_id, exceptions v_code, string v_message){
	statePool[v_id] = v_code;
	messagePool[v_id] = v_message;
};

exceptions PandaPool::getState(unsigned int v_id){
	return statePool.at(v_id);
};

bool PandaPool::packState(unsigned int v_id){
	exceptions t_code = statePool.at(v_id);
	return (t_code == success);
};

string PandaPool::packMessage(unsigned int v_id){
	exceptions t_code = statePool.at(v_id);
	string t_message = messagePool.at(v_id);
	switch(t_code){
		case success:
			t_message = "Success!";
		break;
		case typeError:
			t_message.insert(0, "Type error: ");
		break;
		case operationError:
			t_message.insert(0, "Operation error: ");
		break;
		case parameterError:
			t_message.insert(0, "Parameter error: ");
		break;
		case noCommandError:
			t_message.insert(0, "Command error: ");
		break;
		case nanError:
			t_message.insert(0, "NaN error: ");
		break;
		default:
			t_message.insert(0, "Unknown error: ");
		break;
	}
	return t_message;
};

//Data functions
exceptions PandaPool::save(unsigned int v_id, Local<Value> v_data, const char* v_name){
	setState(v_id, success, "Success!");
    parameterType t_saveType = find(v_name);
    if(t_saveType != p_missing){
    	remove(v_id, v_name);
    }
    parameterType v_type = getType(v_data);
    vec t_vec;
    mat t_mat;
    double t_number;
    string t_string;
    bool t_bool;
    switch(v_type){
    	case p_vector:
			unpackArray(v_id, v_data, t_vec);
			if(!packState(v_id)) break;
			dataVec.insert(pair<string, vec>(v_name, t_vec));
			dataPool.insert(pair<string, parameterType>(v_name, p_vector));
    	break;
    	case p_matrix:
			unpackArray(v_id, v_data, t_mat);
			if(!packState(v_id)) break;
			dataMat.insert(pair<string, mat>(v_name, t_mat));
			dataPool.insert(pair<string, parameterType>(v_name, p_matrix));
    	break;
    	case p_value:
    		t_number = v_data -> NumberValue();
			dataValue.insert(pair<string, double>(v_name, t_number));
			dataPool.insert(pair<string, parameterType>(v_name, p_value));
    	break;
    	case p_string:
    		t_string = string(*(String::Utf8Value(v_data)));
			dataString.insert(pair<string, string>(v_name, t_string));
			dataPool.insert(pair<string, parameterType>(v_name, p_string));
    	break;
    	case p_bool:
    		t_bool = v_data -> BooleanValue();
			dataBool.insert(pair<string, bool>(v_name, t_bool));
			dataPool.insert(pair<string, parameterType>(v_name, p_bool));
    	break;
    	case p_unkown:
    		setState(v_id, parameterError, "Unrecognized data type!");
    	break;
    };    
    return getState(v_id);
};

exceptions PandaPool::saveResult(unsigned int v_id, const char* v_to, const char* v_from){
	setState(v_id, success, "Success!");
    parameterType t_saveType = find(v_to);
    if(t_saveType != p_missing){
    	remove(v_id, v_to);
    }
	parameterType t_type = getType(v_id);
    switch(t_type){
    	case p_vector:
			dataVec.insert(pair<string, vec>(v_to, dataVec.at(v_from)));
			dataPool.insert(pair<string, parameterType>(v_to, p_vector));
    	break;
    	case p_matrix:
			dataMat.insert(pair<string, mat>(v_to, dataMat.at(v_from)));
			dataPool.insert(pair<string, parameterType>(v_to, p_matrix));
    	break;
    	case p_value:
			dataValue.insert(pair<string, double>(v_to, dataValue.at(v_from)));
			dataPool.insert(pair<string, parameterType>(v_to, p_value));
    	break;
    	case p_string:
			dataString.insert(pair<string, string>(v_to, dataString.at(v_from)));
			dataPool.insert(pair<string, parameterType>(v_to, p_string));
    	break;
    	case p_bool:
			dataBool.insert(pair<string, bool>(v_to, dataBool.at(v_from)));
			dataPool.insert(pair<string, parameterType>(v_to, p_bool));
    	break;
    	case p_missing:
    		setState(v_id, operationError, "Variable " + string(v_from) + " missing!");
    	break;
    	case p_unkown:
    		setState(v_id, parameterError, "Unrecognized data type!");
    	break;
    };
    return getState(v_id);
};

exceptions PandaPool::saveResult(unsigned int v_id, const char* v_name){
	setState(v_id, success, "Success!");
    parameterType t_saveType = find(v_name);
    if(t_saveType != p_missing){
    	remove(v_id, v_name);
    }
    parameterType t_type = getType(v_id);
    switch(t_type){
    	case p_vector:
			dataVec.insert(pair<string, vec>(v_name, getVec(v_id)));
			dataPool.insert(pair<string, parameterType>(v_name, p_vector));
    	break;
    	case p_matrix:
			dataMat.insert(pair<string, mat>(v_name, getMat(v_id)));
			dataPool.insert(pair<string, parameterType>(v_name, p_matrix));
    	break;
    	case p_value:
			dataValue.insert(pair<string, double>(v_name, getValue(v_id)));
			dataPool.insert(pair<string, parameterType>(v_name, p_value));
    	break;
    	case p_string:
			dataString.insert(pair<string, string>(v_name, getString(v_id)));
			dataPool.insert(pair<string, parameterType>(v_name, p_string));
    	break;
    	case p_bool:
			dataBool.insert(pair<string, bool>(v_name, getBool(v_id)));
			dataPool.insert(pair<string, parameterType>(v_name, p_bool));
    	break;
    	case p_missing:
    		setState(v_id, operationError, "Result missing!");
    	break;
    	case p_unkown:
    		setState(v_id, parameterError, "Unrecognized data type!");
    	break;
    };
    return getState(v_id);
};

exceptions PandaPool::check(unsigned int v_id, const char* v_name){
	Isolate* t_isolate = getIsolate(v_id);
	setState(v_id, success, "Success!");
	exceptions t_state;
	parameterType t_type = find(v_name);
	switch(t_type){
	    	case p_missing:
	    		t_state = operationError;
	    		setState(v_id, operationError, "Variable " + string(v_name) +" not found!");
	    	break;
	    	default: 
		    	t_state = success;
				typePool[v_id] = t_type;
	    	break;
	}
	return t_state;
};

exceptions PandaPool::get(unsigned int v_id, const char* v_name){
	Isolate* t_isolate = getIsolate(v_id);
	setState(v_id, success, "Success!");
	parameterType t_type = find(v_name);
	switch(t_type){
	    	case p_vector:
	    		vecPool.insert(pair<unsigned int, vec>(v_id, dataVec.at(v_name)));
	    	break;
	    	case p_matrix:
	    		matPool.insert(pair<unsigned int, mat>(v_id, dataMat.at(v_name)));
	    	break;
	    	case p_value:
	    		numberPool.insert(pair<unsigned int, double>(v_id, dataValue.at(v_name)));
	    	break;
	    	case p_string:
	    		stringPool.insert(pair<unsigned int, string>(v_id, dataString.at(v_name)));
	    	break;
	    	case p_bool:
	    		boolPool.insert(pair<unsigned int, bool>(v_id, dataBool.at(v_name)));
	    		break;
	    	case p_missing:
	    		setState(v_id, operationError, "Variable " + string(v_name) +" not found!");
	    	break;
	    	default: 
	    	break;
	}
	typePool[v_id] = t_type;
	return getState(v_id);
};

exceptions PandaPool::remove(unsigned int v_id, const char* v_name){
	setState(v_id, success, "Success!");
    parameterType t_type = find(v_name);
    switch(t_type){
    	case p_vector:
    		dataVec.erase(v_name);
    		dataPool.erase(v_name);
    	break;
    	case p_matrix:
    		dataMat.erase(v_name);
    		dataPool.erase(v_name);
    	break;
    	case p_value:
    		dataValue.erase(v_name);
    		dataPool.erase(v_name);
    	break;
    	case p_string:
    		dataString.erase(v_name);
    		dataPool.erase(v_name);
    	break;
    	case p_bool:
    		dataBool.erase(v_name);
    		dataPool.erase(v_name);
    	break;
    	case p_missing:
    	break;
    }
    return success;
};

exceptions PandaPool::send(unsigned int v_id, OperationData& v_oprData, parameterType v_type, const char* v_from, const char* v_to){
	setState(v_id, success, "Success!");
	parameterType t_type = getType(v_id);
	exceptions t_state = success;
	if(t_type != v_type){
		t_state = parameterError;
		setState(v_id, parameterError, "Wrong parameter type: \'" + string(v_from) + "\'' ");
		return t_state;
	}
	switch(t_type){
    	case p_vector:
    		v_oprData.saveData(dataVec.at(v_from), v_to);
    	break;
    	case p_matrix:
    		v_oprData.saveData(dataMat.at(v_from), v_to);
    	break;
    	case p_value:
    		v_oprData.saveData(dataValue.at(v_from), v_to);
    	break;
    	case p_string:
    		v_oprData.saveData(dataString.at(v_from), v_to);
    	break;
    	case p_bool:
    		v_oprData.saveData(dataBool.at(v_from), v_to);
    	break;
    	case p_missing:
    	break;
	}
    return success;
}

parameterType PandaPool::getType(Local<Value> v_data){
	parameterType t_type = p_unkown;
	if(v_data -> IsArray()){
		Local<Array> v_arr = Local<Array>::Cast(v_data);
		t_type = p_vector;
		if((v_arr -> Get(0)) -> IsArray()){
			t_type = p_matrix;
		}
	}
	if(v_data -> IsNumber()){
		t_type = p_value;
	}
	if(v_data -> IsString()){
		t_type = p_string;
	}
	if(v_data -> IsBoolean()){
		t_type = p_bool;
	}
	return t_type;
};

parameterType PandaPool::getType(unsigned int v_id){
	return typePool.at(v_id);
};

Local<Array> PandaPool::getResultVec(unsigned int v_id){
	Local<Array> t_arr;
	packArray(v_id, getVec(v_id), t_arr);
	return t_arr;
};

Local<Array> PandaPool::getResultMat(unsigned int v_id){
	Local<Array> t_arr;
	packArray(v_id, getMat(v_id), t_arr);
	return t_arr;
};

Local<Number> PandaPool::getResultValue(unsigned int v_id){
	Isolate* t_isolate = getIsolate(v_id);
	return Number::New(t_isolate, getValue(v_id));
};

Local<String> PandaPool::getResultString(unsigned int v_id){
	Isolate* t_isolate = getIsolate(v_id);
	return String::NewFromUtf8(t_isolate, (getString(v_id)).c_str());
};

Local<Boolean> PandaPool::getResultBool(unsigned int v_id){
	Isolate* t_isolate = getIsolate(v_id);
	return Boolean::New(t_isolate, getBool(v_id));
};

vec PandaPool::getVec(unsigned int v_id){
	return vecPool.at(v_id);
};

mat PandaPool::getMat(unsigned int v_id){
	return matPool.at(v_id);
};

double PandaPool::getValue(unsigned int v_id){
	return numberPool.at(v_id);
};

string PandaPool::getString(unsigned int v_id){
	return stringPool.at(v_id);
};

bool PandaPool::getBool(unsigned int v_id){
	return boolPool.at(v_id);
};

exceptions PandaPool::getResult(unsigned int v_id, OperationData& v_data){
	exceptions t_state = v_data.stateCode;
	string t_message = v_data.stateMessage;
	setState(v_id, t_state, t_message);
	Isolate* t_isolate = getIsolate(v_id);
	parameterType t_type = v_data.resultType;
	if(t_state == success){
		switch(t_type){
				case p_vector:
					vecPool.insert(pair<unsigned int, vec>(v_id, v_data.resultVec));
				break;
		    	case p_matrix:
		    		matPool.insert(pair<unsigned int, mat>(v_id, v_data.resultMat));
		    	break;
		    	case p_value:
		    		numberPool.insert(pair<unsigned int, double>(v_id, v_data.resultValue));
		    	break;
		    	case p_string:
		    		stringPool.insert(pair<unsigned int, string>(v_id, v_data.resultString));
		    	break;
		    	case p_bool:
		    		boolPool.insert(pair<unsigned int, bool>(v_id, v_data.resultBool));
		    	break;
		    	case p_missing:
		    	break;
		}
		typePool[v_id] = t_type;
	}
	return t_state;
};

void PandaPool::unpackArray(unsigned int v_id, Local<Value> v_vec, vec& v_data){
	int t_rows = 0;bool t_error = false;
	Local<Array> v_vec_i = Local<Array>::Cast(v_vec);
	t_rows = v_vec_i -> Length();
	v_data = vec(t_rows);
	for(int i = 0; i < t_rows; i++){
		double v_element;
		if(!(v_vec_i -> Get(i) -> IsNumber())){
			t_error = true;
			setState(v_id, nanError, "Element (" + num2str(i) + ") is not a number!");
			break;
		}else{
			v_element = v_vec_i -> Get(i) -> NumberValue();
			v_data(i) = v_element;
		}
		if(t_error) break;
	}
};

void PandaPool::unpackArray(unsigned int v_id, Local<Value> v_mat, mat& v_data){
	int t_rows = 0, t_cols = 0;
	bool t_error = false;
	if(v_mat -> IsArray()){
		Local<Array> v_mat_i = Local<Array>::Cast(v_mat);
		t_rows = v_mat_i -> Length();
		Local<Value> v_ele_j = v_mat_i->Get(0);
		if(v_ele_j -> IsArray()){
			Local<Array> v_mat_j = Local<Array>::Cast(v_ele_j);
			t_cols = v_mat_j -> Length();
		}
	}
	if(t_rows != 0 && t_cols != 0){
		Local<Array> v_mat_i = Local<Array>::Cast(v_mat);
		v_data = mat(t_rows,t_cols);
		for(int i = 0; i < t_rows; i++){
			Local<Array> v_mat_j = Local<Array>::Cast(v_mat_i->Get(i));
			for(int j = 0; j < t_cols; j++){
				double v_element;
				if(!(v_mat_j -> Get(j) -> IsNumber())){
					t_error = true;
					setState(v_id, nanError, "Element (" + num2str(i) + ", " + num2str(j) + ") is not a number!");
					break;
				}else{
					v_element = v_mat_j -> Get(j) -> NumberValue();
					v_data(i, j) = v_element;
				}
			}
			if(t_error) break;
		}
	}else{
		setState(v_id, parameterError, "Unrecognized data type!");
	}
};

void PandaPool::packArray(unsigned int v_id, vec v_vec, Local<Array>& v_arr){
	Isolate* t_isolate = getIsolate(v_id);
	int t_size = v_vec.n_elem;
	v_arr = Array::New(t_isolate);
	for(int i = 0; i < t_size; i++){
		Local<Array> vv_arr = Array::New(t_isolate);
		v_arr->Set(i, Number::New(t_isolate, v_vec(i)));
	}
};

void PandaPool::packArray(unsigned int v_id, mat v_mat, Local<Array>& v_arr){
	Isolate* t_isolate = getIsolate(v_id);
	int t_size[2] = {(int)v_mat.n_rows, (int)v_mat.n_cols};
	v_arr = Array::New(t_isolate);
	for(int i = 0; i < t_size[0]; i++){
		Local<Array> vv_arr = Array::New(t_isolate);
		for(int j = 0; j < t_size[1]; j++){
			vv_arr->Set(j, Number::New(t_isolate, v_mat(i,j)));
		}
		v_arr->Set(i, vv_arr);
	}
};
