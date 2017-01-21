#include "operationdata.h"

OperationData::OperationData(){
	setState(success, "Success!");
}

void OperationData::setState(exceptions v_code, const char * v_message){
	stateCode = v_code;
	stateMessage = v_message;
}

parameterType OperationData::find(const char* v_name){
	map<string, parameterType>::iterator it = dataPool.find(v_name);
	if(it == dataPool.end()){
		return p_missing;
	}else{
		return dataPool.at(v_name);
	}
}

parameterType OperationData::find(unsigned int v_ord){
	return find(num2str(v_ord).c_str());
}

bool OperationData::getData(unsigned int v_ord, vec& v_vec){
	parameterType t_type = find(v_ord);
	bool t_state = true;
	if(t_type == p_missing){
		t_state = false;
		setState(parameterError, "Variable missing!");
	}else{
		if(t_type != p_vector){
			t_state = false;
			setState(typeError, "Mismatched parameter type!");
		}else{			
			v_vec = dataVec.at(num2str(v_ord));
		}
	}
	return t_state;
}

bool OperationData::getData(unsigned int v_ord, mat& v_mat){
	parameterType t_type = find(v_ord);
	bool t_state = true;
	if(t_type == p_missing){
		t_state = false;
		setState(parameterError, "Variable missing!");
	}else{
		if(t_type != p_matrix){
			t_state = false;
			setState(typeError, "Mismatched parameter type!");
		}else{			
			v_mat = dataMat.at(num2str(v_ord));
		}
	}
	return t_state;
}

bool OperationData::getData(unsigned int v_ord, double& v_value){
	parameterType t_type = find(v_ord);
	bool t_state = true;
	if(t_type == p_missing){
		t_state = false;
		setState(parameterError, "Variable missing!");
	}else{
		if(t_type != p_value){
			t_state = false;
			setState(typeError, "Mismatched parameter type!");
		}else{			
			v_value = dataValue.at(num2str(v_ord));
		}
	}
	return t_state;
}

bool OperationData::getData(unsigned int v_ord, string& v_string){
	parameterType t_type = find(v_ord);
	bool t_state = true;
	if(t_type == p_missing){
		t_state = false;
		setState(parameterError, "Variable missing!");
	}else{
		if(t_type != p_string){
			t_state = false;
			setState(typeError, "Mismatched parameter type!");
		}else{			
			v_string = dataString.at(num2str(v_ord));
		}
	}
	return t_state;
}

bool OperationData::getData(unsigned int v_ord, bool& v_bool){
	parameterType t_type = find(v_ord);
	bool t_state = true;
	if(t_type == p_missing){
		t_state = false;
		setState(parameterError, "Variable missing!");
	}else{
		if(t_type != p_bool){
			t_state = false;
			setState(typeError, "Mismatched parameter type!");
		}else{			
			v_bool = dataBool.at(num2str(v_ord));
		}
	}
	return t_state;
}

void OperationData::saveData(vec v_vec, const char* v_name){
	dataVec.insert(pair<string, vec>(v_name, v_vec));
	dataPool.insert(pair<string, parameterType>(v_name, p_vector));
}

void OperationData::saveData(mat v_mat, const char* v_name){
	dataMat.insert(pair<string, mat>(v_name, v_mat));
	dataPool.insert(pair<string, parameterType>(v_name, p_matrix));
}

void OperationData::saveData(double v_value, const char* v_name){
	dataValue.insert(pair<string, double>(v_name, v_value));
	dataPool.insert(pair<string, parameterType>(v_name, p_value));
}

void OperationData::saveData(string v_string, const char* v_name){
	dataString.insert(pair<string, string>(v_name, v_string));
	dataPool.insert(pair<string, parameterType>(v_name, p_string));
}

void OperationData::saveData(bool v_bool, const char* v_name){
	dataBool.insert(pair<string, bool>(v_name, v_bool));
	dataPool.insert(pair<string, parameterType>(v_name, p_bool));
}

void OperationData::setResultType(parameterType v_type){
	resultType = v_type;
}

void OperationData::saveResultV(const vec& v_vec){
	resultVec = v_vec;
}

void OperationData::saveResult(const mat& v_mat){
	resultMat = v_mat;
}

void OperationData::saveResult(const double& v_value){
	resultValue = v_value;
}

void OperationData::saveResult(const string& v_string){
	resultString = v_string;
}

void OperationData::saveResult(const bool& v_bool){
	resultBool = v_bool;
}
