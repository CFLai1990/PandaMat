#include "multiply.h"

Multiply::Multiply(int v_par, parameterType* v_types, parameterType v_type){
	parameterNum = v_par;
	for(int i = 0; i < v_par; i++){
		parameterTypes[i] = v_types[i];
	}
	data.setResultType(v_type);
};

void Multiply::operate(){
	mat t_mat, t_result;
	double t_factor;
	bool success0 = data.getData(0, t_mat);
	bool success1 = data.getData(1, t_factor);
	if(success0 && success1){
		exceptions t_success = get(t_mat, t_factor, t_result);
		if(t_success != success){
			data.setState(operationError, "Unkown error!");
		}else{
			data.saveResult(t_result);
		}
	}
}

exceptions Multiply::get(mat& v_mat, double& v_factor, mat& v_result){
	v_result = v_mat * v_factor;
	return success;
}