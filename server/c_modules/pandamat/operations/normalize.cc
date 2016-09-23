#include "normalize.h"

Normalize::Normalize(int v_par, parameterType* v_types, parameterType v_type){
	parameterNum = v_par;
	for(int i = 0; i < v_par; i++){
		parameterTypes[i] = v_types[i];
	}
	data.setResultType(v_type);
};

void Normalize::operate(){
	mat t_mat, t_mean, t_result;
	matSize t_size;
	bool success = data.getData(0, t_mat);
	if(success){
		t_mean = mean(t_mat);
		t_size = getSize(t_mat);
		t_result = t_mat - repmat(t_mean, t_size.rows, 1);
		data.saveResult(normalise(t_result));
	}
}