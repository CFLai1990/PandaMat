#include "coordinatews.h"

CoordinateWS::CoordinateWS(int v_par, parameterType* v_types, parameterType v_type){
	parameterNum = v_par;
	for(int i = 0; i < v_par; i++){
		parameterTypes[i] = v_types[i];
	}
	data.setResultType(v_type);
};

void CoordinateWS::operate(){
	mat t_mat, t_dist, t_result;bool t_expand;
	bool success0 = data.getData(0, t_mat);
	bool success1 = data.getData(1, t_dist);
	bool success2 = data.getData(2, t_expand);
	if(success0 && success1 && success2){
		get(t_mat, t_dist, t_expand, t_result);
		data.saveResult(t_result);
	}
};

void CoordinateWS::get(mat& v_mat, mat& v_dist, bool v_expand, mat& v_result){
	mat V, t_sub;vec E;uvec indeces;
	eig_sym(E, V, v_dist);
	V = trans(V);
	t_sub.set_size(V.n_rows, V.n_cols);
	v_result.set_size(2, V.n_cols);
	unsigned int t_count = 0;
	for(unsigned int i = 0; i < E.n_elem; i++){
		if(E(i) > pDelta){
			t_sub.row(t_count) = V.row(i);
			t_count++;
		}else{
			E(i) = 0;
		}
	}
	t_sub.set_size(t_count, V.n_cols);
	if(v_expand){
		indeces = stable_sort_index(E, "descend");
	}else{
		indeces = stable_sort_index(E, "ascend");
	}
		// #pragma omp parallel for 
	v_result.row(0) = V.row(indeces(0));
	v_result.row(1) = V.row(indeces(1));
	if(v_expand){
		if(E(indeces(1)) == 0){
			subspaceMDS(v_mat, t_sub, v_expand, v_result);
		}
	}else{
		if(E(indeces(0)) == 0 && E(indeces(1)) == 0){
			if(t_sub.n_elem > 0){
				subspaceMDS(v_mat, t_sub, v_expand, v_result);
			}else{
				oprCoord.get(v_mat, true, v_result);
			}
		}
	}
	v_result = trans(v_result);
};

void CoordinateWS::subspaceMDS(mat& v_data, mat& v_subvec, bool v_expand, mat& v_result){
	mat t_subdata = v_data - (v_data * trans(v_subvec)) * v_subvec, t_cords;
	oprCoord.get(t_subdata, v_expand, t_cords);
	t_cords = trans(t_cords);
	if(v_expand){
		v_result.row(0) = t_cords.row(0);
	}else{
		v_result.row(0) = t_cords.row(0);
		v_result.row(1) = t_cords.row(1);
	}
};
