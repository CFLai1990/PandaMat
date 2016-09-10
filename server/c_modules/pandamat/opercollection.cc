#include "opercollection.h"

OperationResult OperationCollection::find(const char * v_operName){
	string t_name(v_operName);
	OperationResult t_result;
	bool found = false;
	if(t_name == "add"){
		parameterType t_types[2] = {p_matrix, p_matrix};
		oprAdd = Add(2, t_types);
		found = true;
		t_result.operation = &oprAdd;
		return t_result;
	}
	if(t_name == "dot"){
		parameterType t_types[2] = {p_matrix, p_matrix};
		oprDot = Dot(2, t_types);
		found = true;
		t_result.operation = &oprDot;
		return t_result;
	}
	if(t_name == "trans"){
		parameterType t_types[1] = {p_matrix};
		oprTrans = Transpose(2, t_types);
		found = true;
		t_result.operation = &oprTrans;
		return t_result;
	}
	if(!found){
		t_result.stateCode = noCommandError;
		t_result.message = "Command not found!";
		return t_result;
	}
}