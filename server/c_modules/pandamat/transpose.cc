#include "transpose.h"

Transpose::Transpose(int v_par, parameterType* v_types){
	parameterNum = v_par;
	for(int i = 0; i < v_par; i++){
		parameterTypes[i] = v_types[i];
	}
};

OperationResult Transpose::operate(){
	OperationResult t_result;
	t_result.result = trans(data[0]);
	return t_result;
}