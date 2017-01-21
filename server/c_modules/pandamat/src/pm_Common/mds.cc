#include "mds.h"

Mds::Mds(int v_par, parameterType* v_types, parameterType v_type){
	parameterNum = v_par;
	for(int i = 0; i < v_par; i++){
		parameterTypes[i] = v_types[i];
	}
	data.setResultType(v_type);
};

void Mds::operate(){
	mat t_mat, t_result;
	bool success0 = data.getData(0, t_mat);
	if(success0){
		get(t_mat, t_result);
		data.saveResult(t_result);
	}
}


void Mds::get(mat& v_mat, mat& v_result){
	oprCoord.get(v_mat, true, v_result);
	v_result = v_mat * v_result;
	v_result.elem(find(v_result < pDelta)).zeros();
}