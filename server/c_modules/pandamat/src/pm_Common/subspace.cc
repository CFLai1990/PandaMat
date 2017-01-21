#include "subspace.h"

Subspace::Subspace(int v_par, parameterType* v_types, parameterType v_type){
	parameterNum = v_par;
	for(int i = 0; i < v_par; i++){
		parameterTypes[i] = v_types[i];
	}
	data.setResultType(v_type);
};

void Subspace::operate(){
	mat t_mat, t_result;vec t_sub;
	bool success0 = data.getData(0, t_mat);
	bool success1 = data.getData(1, t_sub);
	if(success0 && success1){
		exceptions t_success = get(t_mat, t_sub, t_result);
		if(t_success != success){
			data.setState(operationError, "Wrong vector size!");
		}else{
			data.saveResult(t_result);
		}
	}
}


exceptions Subspace::get(mat& v_data, vec& v_sub, mat& v_subdata){
	matSize t_size = getSize(v_data);
	if(t_size.cols != v_sub.n_elem){
		return operationError;
	}else{
		unsigned int t_i = 0;
		v_subdata.set_size(t_size.rows, t_size.cols);
		for(unsigned int i = 0; i < v_sub.n_elem; i++){
			if(v_sub(i) >= 1){
				v_subdata.col(t_i) = v_data.col(i);
				t_i ++;
			}
		}
		v_subdata.set_size(t_size.rows, t_i);
		return success;
	}
}