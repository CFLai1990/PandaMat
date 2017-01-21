#include "kmeans.h"

Kmeans::Kmeans(int v_par, parameterType* v_types, parameterType v_type){
	parameterNum = v_par;
	for(int i = 0; i < v_par; i++){
		parameterTypes[i] = v_types[i];
	}
	data.setResultType(v_type);
};

void Kmeans::operate(){
	mat t_mat;vec t_result;double t_dk;
	bool success0 = data.getData(0, t_mat);
	bool success1 = data.getData(1, t_dk);
	if(success0 && success1){
		t_mat = trans(t_mat);
		bool t_state = get(t_mat, (unsigned int)t_dk, t_result);
		if(!t_state){
			data.setState(operationError, "K-means failed!");
		}else{
			data.saveResultV(t_result);
		}
	}
}

bool Kmeans::get(mat& v_mat, unsigned int v_k, vec& v_result){
	mat t_means;
	bool t_state = kmeans(t_means, v_mat, v_k, static_subset, 10, false);
	if(!t_state){
		return false;
	}else{
		matSize t_size = getSize(v_mat), t_mSize = getSize(t_means);
		v_result.set_size(t_size.cols);
		for(unsigned int i = 0; i < (unsigned int)t_size.cols; i++){
			int t_i = assign(v_mat.col(i), t_means, t_mSize.cols);
			v_result(i) = t_i;
		}
		return true;
	}
}

int Kmeans::assign(vec v_vec, mat& v_mat, int v_cols){
	vec t_norms;
	t_norms.set_size(v_cols);
	for(int i = 0; i < v_cols; i++){
		vec t_vec = v_vec - v_mat.col(i);
		t_norms(i) = norm(t_vec, 2);
	}
	return (int)t_norms.index_max();
}