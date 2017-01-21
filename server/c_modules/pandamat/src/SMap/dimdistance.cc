#include "dimdistance.h"

DimDistance::DimDistance(int v_par, parameterType* v_types, parameterType v_type){
	parameterNum = v_par;
	for(int i = 0; i < v_par; i++){
		parameterTypes[i] = v_types[i];
	}
	data.setResultType(v_type);
};

void DimDistance::operate(){
	mat t_mat, t_result;
	bool success0 = data.getData(0, t_mat);
	if(success0){
		exceptions t_success = get(t_mat, t_result);
		if(t_success != success){
			data.setState(operationError, "Unkown error!");
		}else{
			data.saveResult(t_result);
		}
	}
}

exceptions DimDistance::get(mat& v_codes, mat& v_result){
	matSize t_size = getSize(v_codes);
	v_result = zeros<mat>(t_size.rows, t_size.rows);
	#pragma omp parallel for 
	for(unsigned int i = 0; i < t_size.rows - 1; i++){
		rowvec t_code_i = v_codes.row(i);
		for(unsigned int j = i+1; j < t_size.rows; j++){
			rowvec t_code_j = v_codes.row(j);
			double t_distance = getDistance(t_code_i, t_code_j);
			v_result(i,j) = v_result(j,i) = t_distance;
		}
	}
	return success;
}

double DimDistance::getDistance(rowvec& v_code_i, rowvec& v_code_j){
	double t_dist = 0;
	unsigned int t_items = v_code_i.n_elem;
	for(unsigned int i = 0; i < t_items; i++){
		if(v_code_i[i] != v_code_j[i]){
			t_dist++;
		}
	}
	t_dist /= t_items;
	return t_dist;
}