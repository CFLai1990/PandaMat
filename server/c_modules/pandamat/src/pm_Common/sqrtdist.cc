#include "sqrtdist.h"

SqrtDist::SqrtDist(int v_par, parameterType* v_types, parameterType v_type){
	parameterNum = v_par;
	for(int i = 0; i < v_par; i++){
		parameterTypes[i] = v_types[i];
	}
	data.setResultType(v_type);
};

void SqrtDist::operate(){
	mat t_mat, t_result;
	bool t_success = data.getData(0, t_mat);
	if(t_success == success){
		exceptions t_success1 = get(t_mat, t_result);
		if(t_success1 == success){
			data.saveResult(t_result);
		}else{			
			data.setState(operationError, "Unknown error!");
		}
	}
}

exceptions SqrtDist::get(mat& v_mat, mat& v_result){
	unsigned int t_size = (getSize(v_mat)).rows;
	v_result = zeros<mat>(t_size, t_size);
	#pragma omp parallel for 
	for(unsigned int i = 0; i < t_size; i++){
		for(unsigned int j = i+1; j < t_size; j++){
			v_result(i,j) = v_result(j,i) = arma::norm(v_mat.row(i)-v_mat.row(j), 2);
		}
	}
	return success;
}