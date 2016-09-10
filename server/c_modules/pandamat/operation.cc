#include "operation.h"

Operation::Operation(int v_par, parameterType* v_types){
	parameterNum = v_par;
	for(int i = 0; i < v_par; i++){
		parameterTypes[i] = v_types[i];
	}
};

OperationResult Operation::operate(){
	OperationResult null_result;
	null_result.stateCode = unkownError;
	null_result.message = "operation not found!";
	return null_result;
}

matSize Operation::getSize(mat v_data){
	matSize t_size;
	t_size.rows = (int)v_data.n_rows;
	t_size.cols = (int)v_data.n_cols;
	return t_size;
}