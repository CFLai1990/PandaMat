#include "dot.h"

Dot::Dot(int v_par, parameterType* v_types){
	parameterNum = v_par;
	for(int i = 0; i < v_par; i++){
		parameterTypes[i] = v_types[i];
	}
};

OperationResult Dot::operate(){
	OperationResult t_result;
	matSize t_size0 = getSize(data[0]);
	matSize t_size1 = getSize(data[1]);
	if(t_size0.cols != t_size1.rows){
		t_result.stateCode = operationError;
		t_result.message = "The matrices are mismatched!";
	}else{
		t_result.result = data[0] * data[1];
	}
	return t_result;
}