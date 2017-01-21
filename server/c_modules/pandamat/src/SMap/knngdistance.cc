#include "knngdistance.h"

KNNGDistance::KNNGDistance(int v_par, parameterType* v_types, parameterType v_type){
	parameterNum = v_par;
	for(int i = 0; i < v_par; i++){
		parameterTypes[i] = v_types[i];
	}
	data.setResultType(v_type);
};

void KNNGDistance::operate(){
	mat t_mat, t_result;double t_k;
	bool success0 = data.getData(0, t_mat);
	bool success1 = data.getData(1, t_k);
	if(success0 && success1){
		exceptions t_success = get(t_mat, t_k, t_result);
		if(t_success != success){
			data.setState(operationError, "Unkown error!");
		}else{
			data.saveResult(t_result);
		}
	}
}

exceptions KNNGDistance::get(mat& v_graphs, const double& v_k, mat& v_result){
	matSize t_size = getSize(v_graphs);
	v_result = zeros<mat>(t_size.rows, t_size.rows);
	#pragma omp parallel for 
	for(unsigned int i = 0; i < t_size.rows - 1; i++){
		rowvec t_graph_i = v_graphs.row(i);
		for(unsigned int j = i+1; j < t_size.rows; j++){
			rowvec t_graph_j = v_graphs.row(j);
			double t_distance = getDistance(t_graph_i, t_graph_j, v_k);
			v_result(i,j) = v_result(j,i) = t_distance;
		}
	}
	return success;
}

double KNNGDistance::getDistance(rowvec& v_graph_i, rowvec& v_graph_j, const double& v_k){
	double t_dist = 0;
	unsigned int t_items = (v_graph_i.n_elem) / (unsigned int)(v_k), t_k = (unsigned int) v_k;
	#pragma omp parallel for 
	for(unsigned int i = 0; i < t_items; i++){
		unsigned int t_start = i * t_k, t_end = (i+1) * t_k - 1;
		vec t_i = trans(v_graph_i.cols(t_start, t_end));
		vec t_j = trans(v_graph_j.cols(t_start, t_end));
		t_dist += compare(t_i, t_j, t_k);
	}
	t_dist /= t_items;
	return t_dist;
}

double KNNGDistance::compare(vec& v_nn_i, vec& v_nn_j, const unsigned int& v_k){
	unsigned int t_same = 0;
	for(unsigned int i = 0; i < v_k; i++){
		unsigned int t_target = v_nn_i(i);
		for(unsigned int j = 0; j < v_k; j++){
			if(v_nn_j(j) == t_target){
				t_same ++;
				break;
			}
		}
	}
	return (1.0 - (double)(t_same) / (2 * v_k - t_same));
}
