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
	bool success = data.getData(0, t_mat);
	int t_size;
	if(success){
		t_size = (getSize(t_mat)).rows;
		t_result = zeros<mat>(t_size, t_size);
		#pragma omp parallel for 
		for(int i = 0; i < t_size; i++){
			for(int j = i+1; j < t_size; j++){
				t_result(i,j) = t_result(j,i) = arma::norm(t_mat.row(i)-t_mat.row(j), 2);
			}
		}
		data.saveResult(t_result);
	}
}