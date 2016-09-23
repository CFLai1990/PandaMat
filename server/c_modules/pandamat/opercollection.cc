#include "opercollection.h"

OperationCollection::OperationCollection(){
}

OperationResult OperationCollection::create(unsigned int v_id, const char * v_operName){
	string t_name(v_operName);
	OperationResult t_result;
	bool found = false;
	if(t_name == "add"){
		found = true;
		parameterType t_types[2] = {p_matrix, p_matrix};
		parameterType t_type = p_matrix;
		oprAdd.insert(pair<unsigned int, Add>(v_id, Add(2, t_types, t_type)));
		t_result.operation = &(oprAdd.at(v_id));
	}
	if(t_name == "dot"){
		found = true;
		parameterType t_types[2] = {p_matrix, p_matrix};
		parameterType t_type = p_matrix;
		oprDot.insert(pair<unsigned int, Dot>(v_id, Dot(2, t_types, t_type)));
		t_result.operation = &(oprDot.at(v_id));
	}
	if(t_name == "trans"){
		found = true;
		parameterType t_types[1] = {p_matrix};
		parameterType t_type = p_matrix;
		oprTrans.insert(pair<unsigned int, Transpose>(v_id, Transpose(1, t_types, t_type)));
		t_result.operation = &(oprTrans.at(v_id));
	}
	if(t_name == "normalize"){
		found = true;
		parameterType t_types[1] = {p_matrix};
		parameterType t_type = p_matrix;
		oprNorm.insert(pair<unsigned int, Normalize>(v_id, Normalize(1, t_types, t_type)));
		t_result.operation = &(oprNorm.at(v_id));
	}
	if(t_name == "sqrtDist"){
		found = true;
		parameterType t_types[1] = {p_matrix};
		parameterType t_type = p_matrix;
		oprDist.insert(pair<unsigned int, SqrtDist>(v_id, SqrtDist(1, t_types, t_type)));
		t_result.operation = &(oprDist.at(v_id));
	}
	if(!found){
		t_result.stateCode = noCommandError;
		t_result.stateMessage = "Command not found!";
	}else{
		oprCommands.insert(pair<unsigned int, string>(v_id, t_name));
	}
	return t_result;
}

void OperationCollection::destroy(unsigned int v_id){
	string t_command = oprCommands.at(v_id);
	if(t_command == "add"){
		oprAdd.erase(v_id);
	}
	if(t_command == "dot"){
		oprDot.erase(v_id);
	}
	if(t_command == "trans"){
		oprTrans.erase(v_id);
	}
	if(t_command == "normalize"){
		oprNorm.erase(v_id);
	}
	if(t_command == "sqrtDist"){
		oprDist.erase(v_id);
	}
	oprCommands.erase(v_id);
}