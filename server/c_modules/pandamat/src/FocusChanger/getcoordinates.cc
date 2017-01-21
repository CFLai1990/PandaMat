#include "getcoordinates.h"

GetCoordinates::GetCoordinates(int v_par, parameterType* v_types, parameterType v_type){
	parameterNum = v_par;
	for(int i = 0; i < v_par; i++){
		parameterTypes[i] = v_types[i];
	}
	data.setResultType(v_type);
};

void GetCoordinates::operate(){
	vec t_focus, t_sub; bool t_centroid;
	string t_opti;
	mat t_mat, t_subdata, t_result;
	bool success0 = data.getData(0, t_focus);
	bool success1 = data.getData(1, t_opti);
	bool success2 = data.getData(2, t_sub);
	bool success3 = data.getData(3, t_mat);
	bool success4 = data.getData(4, t_centroid);
	if(success0 && success1 && success2 && success3 && success4){
		exceptions success5 = oprSubspace.get(t_mat, t_sub, t_subdata);
		if(success5 == success){
			exceptions t_state = get(t_subdata, t_focus, t_opti, t_centroid, t_result);
			if(t_state != success){
				data.setState(operationError, "Unknown error!");
			}else{
				data.saveResult(t_result);
			}
		}else{
			data.setState(operationError, "Unknown error!");
		}
	}
}

exceptions GetCoordinates::get(mat& v_sub, vec& v_focus, string& v_opti, bool v_centroid, mat& v_result){
	mat t_focusdata, t_center, t_others, t_matrix;
	exceptions t_state = operationError;
	t_state = focus(v_sub, v_focus, t_focusdata, true);
	if(t_state != success){
		return t_state;
	}
	if(v_opti == "Expand" || v_opti == "Compress"){
		cout << t_focusdata.n_rows << " " << v_opti << endl;
		if(t_focusdata.n_rows > 0){
			if(t_focusdata.n_rows > 1){
				if(v_centroid){
					t_center = t_focusdata - repmat(mean(t_focusdata, 0), t_focusdata.n_rows, 1);
					t_matrix = t_center.t() * t_center;
				}else{
					t_matrix = zeros(t_focusdata.n_cols, t_focusdata.n_cols);
					for(unsigned int i = 0; i < t_focusdata.n_rows; i++){
						t_center = t_focusdata - repmat(t_focusdata.row(i), t_focusdata.n_rows, 1);
						t_matrix += t_center.t() * t_center;
					}
				}
			}else{
				mat t_center = v_sub - repmat(t_focusdata, v_sub.n_rows, 1);
				t_matrix = t_center.t() * t_center;
			}
		}else{
			t_matrix = v_sub.t() * v_sub;
		}
		oprCoordWS.get(v_sub, t_matrix, (v_opti == "Expand"), v_result);
	}
	if(v_opti == "Separate"){
		t_state = focus(v_sub, v_focus, t_others, false);
		if(t_state != success){
			return t_state;
		}
		if(v_centroid){
			t_center = t_others - repmat(mean(t_focusdata, 0), t_others.n_rows, 1);
			t_matrix = t_center.t() * t_center;
		}else{
			t_matrix = zeros(t_others.n_cols, t_others.n_cols);
			for(unsigned int i = 0; i < t_focusdata.n_rows; i++){
				t_center = t_others - repmat(t_focusdata.row(i), t_others.n_rows, 1);
				t_matrix += t_center.t() * t_center;
			}
		}
		oprCoordWS.get(v_sub, t_matrix, true, v_result);
	}
	return t_state;
}

exceptions GetCoordinates::focus(mat& v_data, vec& v_focus, mat& v_result, bool v_sign = true){
	if(v_sign){
		v_result.set_size(v_focus.n_elem, v_data.n_cols);
		for(unsigned int i = 0; i < v_focus.n_elem; i++){
			v_result.row(i) = v_data.row(v_focus(i));
		}
	}else{
		unsigned int t_count = 0;
		v_result.set_size(v_data.n_rows - v_focus.n_elem, v_data.n_cols);
		for(unsigned int i = 0; i < v_data.n_rows; i++){
			uvec t_ind = find(v_focus == i);
			if(t_ind.is_empty()){
				v_result.row(t_count) = v_data.row(i);
				t_count++;
			}
		}
	}
	return success;
}