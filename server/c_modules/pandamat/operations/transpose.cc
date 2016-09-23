#include "transpose.h"

Transpose::Transpose(int v_par, parameterType* v_types, parameterType v_type){
	parameterNum = v_par;
	for(int i = 0; i < v_par; i++){
		parameterTypes[i] = v_types[i];
	}
	data.setResultType(v_type);
};

void Transpose::operate(){
	mat t_mat;
	bool success = data.getData(0, t_mat);
	if(success){
		data.saveResult(trans(t_mat));
	}
}