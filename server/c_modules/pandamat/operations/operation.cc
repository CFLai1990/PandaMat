#include "operation.h"

Operation::Operation(int v_par, parameterType* v_types, parameterType v_type){
	parameterNum = v_par;
	for(int i = 0; i < v_par; i++){
		parameterTypes[i] = v_types[i];
	}
	data.setResultType(v_type);
};

void Operation::operate(){
	setResult(unkownError, "operation not found!");
}

matSize Operation::getSize(mat v_data){
	matSize t_size;
	t_size.rows = (int)v_data.n_rows;
	t_size.cols = (int)v_data.n_cols;
	return t_size;
}

void Operation::setResult(exceptions v_code, string v_message){
	data.stateCode = v_code;
	data.stateMessage = v_message;
}