#include "projection.h"
#include "tapkee/tapkee.hpp"
#include "tapkee/callbacks/dummy_callbacks.hpp"
using namespace tapkee;

Projection::Projection(int v_par, parameterType* v_types, parameterType v_type){
	parameterNum = v_par;
	for(int i = 0; i < v_par; i++){
		parameterTypes[i] = v_types[i];
	}
	data.setResultType(v_type);
};

void Projection::operate(){
	mat t_dist, t_result;
	string t_type;
	double t_dim;
	bool success0 = data.getData(0, t_dist);
	bool success1 = data.getData(1, t_type);
	bool success2 = data.getData(2, t_dim);
	if(success0 && success1 && success2){
		exceptions t_success = get(t_dist, t_type, (unsigned int)t_dim, t_result);
		if(t_success != success){
			data.setState(operationError, "Unkown error!");
		}else{
			data.saveResult(t_result);
		}
	}
}

exceptions Projection::get(mat& v_mat, string& v_type, unsigned int v_dim, mat& v_result){
	struct MyDistanceCallback
	{
		MyDistanceCallback(mat& vv_dist) : myDist(vv_dist){ };
		mat& myDist;
	    ScalarType distance(IndexType l, IndexType r) { return myDist(l, r); }
	};
	struct MyFeatureCallback
	{
		MyFeatureCallback(mat& vv_feature) : myFeature(vv_feature){ };
		mat& myFeature;
		IndexType dimension(){
			return myFeature.n_cols;
		}
		void vector(IndexType l, DenseVector& v_vec) {
			rowvec t_vec = myFeature.row(l);
			for(unsigned int i = 0; i < t_vec.n_elem; i++){
				v_vec(i) = t_vec(i);
			}
		}
	};
	matSize t_size = getSize(v_mat);
    vector<ScalarType> indices(t_size.rows);
	unsigned int t_projdim = v_dim;
	for(unsigned int i=0; i<t_size.rows; i++){
		indices[i] = i;
	}
	bool t_found = false;
	if(v_type == "t-SNE"){
		MyFeatureCallback t_d(v_mat);
		TapkeeOutput output;
		t_found = true;
	    output = tapkee::initialize() 
	   	.withParameters((method=tDistributedStochasticNeighborEmbedding,target_dimension=t_projdim))
	   	.withFeatures(t_d)
	   	.embedUsing(indices);
	   	v_result = zeros(t_size.rows, t_projdim);
	   	copyArray<DenseMatrix, mat>(output.embedding, v_result, t_size.rows, t_projdim);
	}
	if(v_type == "MDS"){
		MyDistanceCallback t_d(v_mat);
		TapkeeOutput output;
		t_found = true;
	    output = tapkee::initialize() 
	   	.withParameters((method=MultidimensionalScaling,target_dimension=t_projdim))
	   	.withDistance(t_d)
	   	.embedUsing(indices);
	   	v_result = zeros(t_size.rows, t_projdim);
	   	copyArray<DenseMatrix, mat>(output.embedding, v_result, t_size.rows, t_projdim);
	}
	if(v_type == "MDS_Precise"){
		mat t_cent = zeros(t_size.rows, t_size.cols),
			t_one = ones(1, t_size.cols),
			U, V, t_s = zeros(t_projdim, t_projdim),
			t_eye;
		vec s;
		t_found = true;
		t_cent = t_eye.eye(t_size.rows, t_size.cols) - t_one.t() * t_one / t_size.cols;
		t_cent = t_cent.t() * v_mat * t_cent / (-2);
		svd_econ(U, s, V, t_cent);
		V = (U + V) / 2;
		s.set_size(t_projdim);
		t_s.diag() = s;
		v_result = V.cols(0, t_projdim - 1) * t_s;
	}
	if(!t_found){
		return operationError;
	}
	for(unsigned int i = 0; i < t_projdim; i++){
		colvec t_col = v_result.col(i);
		if(t_col.max() == t_col.min()){
			v_result.col(i) = zeros<vec>(t_size.rows);
		}else{
			v_result.col(i) = (t_col - t_col.min())/(t_col.max() - t_col.min());
		}
	}
	return success;
}
