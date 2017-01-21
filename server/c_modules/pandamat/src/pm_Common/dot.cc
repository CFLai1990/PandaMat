#include "dot.h"

Dot::Dot(int v_par, parameterType* v_types, parameterType v_type){
	parameterNum = v_par;
	for(int i = 0; i < v_par; i++){
		parameterTypes[i] = v_types[i];
	}
	data.setResultType(v_type);
};

void Dot::operate(){
	mat t_mat0, t_mat1;
	bool success0 = data.getData(0, t_mat0);
	bool success1 = data.getData(1, t_mat1);
	matSize t_size0, t_size1;
	if(success0 && success1){
		t_size0 = getSize(t_mat0);
		t_size1 = getSize(t_mat1);
		if(t_size0.cols != t_size1.rows){
			data.setState(operationError, "Matrices are mismatched!");
		}else{
			data.saveResult(t_mat0 * t_mat1);
		}
	}
}