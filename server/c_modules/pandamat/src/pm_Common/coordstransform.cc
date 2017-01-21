#include "coordstransform.h"

CoordinateTransform::CoordinateTransform(int v_par, parameterType* v_types, parameterType v_type){
	parameterNum = v_par;
	for(int i = 0; i < v_par; i++){
		parameterTypes[i] = v_types[i];
	}
	data.setResultType(v_type);
};

void CoordinateTransform::operate(){
	mat t_mat1, t_mat2, t_result;
	double t_frames;
	bool success0 = data.getData(0, t_mat1);
	bool success1 = data.getData(1, t_mat2);
	bool success2 = data.getData(2, t_frames);
	if(success0 && success1 && success2){
		exceptions t_state = get(t_mat1, t_mat2, t_frames, t_result);
		if(t_state != success){
			data.setState(operationError, "Unknown error!");
		}else{
			data.saveResult(t_result);
		}
	}
}

exceptions CoordinateTransform::get(mat& v_mat1, mat& v_mat2, unsigned int v_frames, mat& v_result){
	unsigned int origDims = v_mat1.n_rows, projDim = v_mat1.n_cols;
	v_result.zeros(1,1);
	mat U, V, G1, G2, rotG;
	vec s;
	svd(U, s, V, trans(v_mat1) * v_mat2);
	rotG = V * trans(U);
	if((rotG(0,0) - rotG(1,1)) * (rotG(0,0) - rotG(1,1)) > 1e-10 || rotG(1,0) * rotG(0,1) > 0){
		colvec t_c0 = v_mat2.col(0), t_c1 = v_mat2.col(1);
		v_mat2.col(0) = t_c1;
		v_mat2.col(1) = t_c0;
		svd(U, s, V, trans(v_mat1) * v_mat2);
		rotG = V * trans(U);
	}
	G1 = trans(v_mat1 * U);
	G2 = trans(v_mat2 * V);
	mat B(s.n_elem * 2, origDims),
		t_sameIDs(s.n_elem, 2);
	vec t_sameAngles(s.n_elem);
	bool t_different = false;
	unsigned int t_countB = 0;
	for(unsigned int i = 0; i < s.n_elem; i++){
		rowvec t_g1 = G1.row(i),
			   t_g2 = G2.row(i);
		B.row(t_countB) = t_g1;
		t_countB++;
		if(abs(s(i)) < 1 - 1e-10){
			t_different = true;
			mat mid_mat = ((t_g1 * trans(t_g2)) / (t_g1 * trans(t_g1)));
			rowvec t_G = t_g2 - t_g1 * mid_mat(0,0);
			t_G = t_G / norm(t_G, 2);
			B.row(t_countB) = t_G;
			t_countB++;
			t_sameIDs(i, 0) = t_countB - 2;
			t_sameIDs(i, 1) = t_countB - 1;
			t_sameAngles(i) = acos(s(i));
		}else{
			t_sameIDs(i, 0) = t_countB - 1;
			t_sameIDs(i, 1) = NAN;
			t_sameAngles(i) = NAN;
		}
	}
	resize(B, t_countB, B.n_cols);
	if(t_different){
		mat t_mat(s.n_elem, origDims);
		vec t_signs(s.n_elem);
		v_result.zeros(v_frames, origDims * projDim);
		for(unsigned int i = 0; i < t_sameIDs.n_rows; i++){
			rowvec t_sameid = t_sameIDs.row(i);
			if(std::isnan((double)t_sameid(1))){
				t_mat.row(i) = B.row(t_sameid(0));
			}else{
				double t_angle = t_sameAngles(i);
				rowvec t_start = B.row(t_sameid(0)), t_end = B.row(t_sameid(1)),
					   t_tryG2 = t_start * cos(t_angle) + t_end * sin(t_angle),
					   t_rowG2 = G2.row(i);
				double t_residule = norm(t_tryG2 - t_rowG2, 2);
				t_signs(i) = t_residule < 1e-10?1:-1;
			}
		}
		for(unsigned int j = 0; j < v_frames; j++){
			for(unsigned int i = 0; i < t_sameIDs.n_rows; i++){
				rowvec t_sameid = t_sameIDs.row(i);
				if(!std::isnan((double)t_sameid(1))){
					double t_angle = (j+1) / (double)v_frames * t_sameAngles(i);
					rowvec t_start = B.row(t_sameid(0)), t_end = B.row(t_sameid(1));
					t_mat.row(i) = t_start * cos(t_angle) + t_signs(i) * t_end * sin(t_angle);
				}
			}
			mat t_frameMat = trans(U * t_mat);
			// if(j == v_frames - 1){
			// 	mat t_try = trans(U * G2);
			// 	printMatrix(t_frameMat);
			// 	printMatrix(t_try);
			// }
			t_frameMat.reshape(1, s.n_elem * origDims);
			v_result.row(j) = t_frameMat.row(0);
		}
	}
	return success;
}