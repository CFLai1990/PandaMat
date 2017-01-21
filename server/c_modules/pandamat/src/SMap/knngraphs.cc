#include "knngraphs.h"

KNNGraphs::KNNGraphs(int v_par, parameterType* v_types, parameterType v_type){
	parameterNum = v_par;
	for(int i = 0; i < v_par; i++){
		parameterTypes[i] = v_types[i];
	}
	data.setResultType(v_type);
};

void KNNGraphs::operate(){
	double t_k;
	mat t_codes, t_data, t_result;
	string t_type;
	bool success0 = data.getData(0, t_codes);
	bool success1 = data.getData(1, t_data);
	bool success2 = data.getData(2, t_k);
	bool success3 = data.getData(3, t_type);
	if(success0 && success1 && success2 && success3){
		exceptions t_state = get(t_data, t_codes, (unsigned int)t_k, t_result, t_type);
		if(t_state != success){
			data.setState(operationError, "Unknown error!");
		}else{
			data.saveResult(t_result);
		}
	}
}

exceptions KNNGraphs::get(mat& v_data, mat& v_codes, unsigned int v_k, mat& v_result, string v_type = "approximate"){
	matSize t_size = getSize(v_codes), t_dsize = getSize(v_data);
	bool t_success = true;
	v_result = zeros<mat>(t_size.rows, t_dsize.rows * v_k);
	#pragma omp parallel for 
	for(unsigned int i = 0; i < t_size.rows; i++){
		vec t_code = trans(v_codes.row(i));
		mat t_subdata, t_subdist, t_KNNG;
		exceptions success0 = oprSub.get(v_data, t_code, t_subdata);
		if(success0 == success){
			exceptions success1 = oprKNNG.get(t_subdata, v_k, t_KNNG, v_type);
			if(success1 == success){
				mat tt_KNNV;
				inplace_trans(t_KNNG);
				tt_KNNV = reshape(t_KNNG, 1, t_dsize.rows * v_k);
				matSize tt_size = getSize(tt_KNNV);
				tt_size = getSize(v_result);
				v_result.row(i) = tt_KNNV.row(0);
			}else{
				t_success = false;
			}
		}else{
			t_success = false;
		}
	}
	if(t_success){
		return success;
	}else{
		return operationError;
	}
}
