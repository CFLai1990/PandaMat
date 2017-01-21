#include "coordinate.h"

Coordinate::Coordinate(int v_par, parameterType* v_types, parameterType v_type){
	parameterNum = v_par;
	for(int i = 0; i < v_par; i++){
		parameterTypes[i] = v_types[i];
	}
	data.setResultType(v_type);
};

void Coordinate::operate(){
	mat t_mat, t_result;bool t_expand;
	bool success0 = data.getData(0, t_mat);
	bool success1 = data.getData(1, t_expand);
	if(success0 && success1){
		get(t_mat, t_expand, t_result);
		data.saveResult(t_result);
	}
}

void Coordinate::get(mat& v_mat, bool v_expand, mat& v_result){
	mat U, V;vec s;
	matSize t_size = getSize(v_mat);
	uvec indeces;
	svd_econ(U, s, V, v_mat, "right");
	V = trans(V);
	v_result.set_size(2, t_size.cols);
	if(v_expand){
		indeces = stable_sort_index(s, "descend");
	}else{
		indeces = stable_sort_index(s, "ascend");
	}
	v_result.row(0) = V.row(indeces(0));
	v_result.row(1) = V.row(indeces(1));
	v_result = trans(v_result);
}