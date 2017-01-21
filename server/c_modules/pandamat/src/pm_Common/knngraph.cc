#include "knngraph.h"

KNNGraph::KNNGraph(int v_par, parameterType* v_types, parameterType v_type){
	parameterNum = v_par;
	for(int i = 0; i < v_par; i++){
		parameterTypes[i] = v_types[i];
	}
	data.setResultType(v_type);
};

void KNNGraph::operate(){
	double t_k;
	mat t_dist, t_result;
	string t_type;
	bool success0 = data.getData(0, t_dist);
	bool success1 = data.getData(1, t_k);
	bool success2 = data.getData(2, t_type);
	if(success0 && success1 && success2){
		exceptions t_state = get(t_dist, (unsigned int)t_k, t_result, t_type);
		if(t_state != success){
			data.setState(operationError, "Unknown error!");
		}else{
			data.saveResult(t_result);
		}
	}
}

exceptions KNNGraph::get(mat& v_data, unsigned int v_k, mat& v_result, string v_type = "approximate"){
    unsigned int t_k = v_k + 1; // To exclude the point itself, we need to find k+1 'neighbors'
	matSize t_size = getSize(v_data);
	double* t_data = new double[t_size.rows * t_size.cols];
	double* t_query = new double[t_size.rows * t_size.cols];
	unpackMatrix(v_data, t_size, t_data);
	unpackMatrix(v_data, t_size, t_query);
	Matrix<double> dataset(t_data, t_size.rows, t_size.cols);
    Matrix<double> query(t_query, t_size.rows, t_size.cols);
    Matrix<int> indices(new int[query.rows*t_k], query.rows, t_k);
    Matrix<double> dists(new double[query.rows*t_k], query.rows, t_k);
    AutotunedIndexParams t_precise(1.0);
    Index<L2<double> > index(dataset, t_precise);
    if(v_type.compare("precise") != 0){
    	index = *(new Index<L2<double> >(dataset, flann::KDTreeIndexParams(8)));
    }
    index.buildIndex();
    index.knnSearch(query, indices, dists, t_k, flann::SearchParams(FLANN_CHECKS_UNLIMITED));
    packMatrix(indices, v_result);

    delete[] dataset.ptr();
    delete[] query.ptr();
    delete[] indices.ptr();
    delete[] dists.ptr();
	return success;
}

exceptions KNNGraph::unpackMatrix(mat& v_data, matSize& v_size, double* v_result){
	for(unsigned int i = 0; i < v_size.rows; i++){
		for(unsigned int j = 0; j < v_size.cols; j++){
			v_result[i*v_size.cols + j] = v_data(i,j);
		}
	}
	return success;
}

exceptions KNNGraph::packMatrix(Matrix<int> v_matrix, mat& v_result){
	v_result = zeros<mat>(v_matrix.rows, v_matrix.cols - 1);
	#pragma omp parallel for 
	for(unsigned int i = 0; i < v_matrix.rows; i++){
		unsigned int t_ind = 0;
		for(unsigned int j = 0; j < v_matrix.cols; j++){
			if((unsigned int)(v_matrix[i][j]) == i || t_ind == v_matrix.cols - 1){//exclude the point itself
				// if(t_ind == v_matrix.cols - 1){
				// 	cout << i << ": ";
				// 	for(unsigned int k = 0; k < v_matrix.cols; k++){
				// 		cout << v_matrix[i][k] << " , ";
				// 	}
				// 	cout << endl;
				// }
				continue;
			}
			v_result(i,t_ind) = (double)v_matrix[i][j];
			t_ind ++;
		}
	}
	return success;
}
