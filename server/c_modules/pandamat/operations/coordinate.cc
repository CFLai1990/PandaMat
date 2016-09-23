#include "coordinate.h"

Coordinate::Coordinate(int v_par, parameterType* v_types, parameterType v_type){
	parameterNum = v_par;
	for(int i = 0; i < v_par; i++){
		parameterTypes[i] = v_types[i];
	}
	data.setResultType(v_type);
};

void Coordinate::operate(){
	// mat U;
	// vec s;
	// mat V;
	// svd(U,s,V,data[0]);
	// V = trans(V);
	// s = s%s;
	// vec sOrd = s;


	// OperationResult t_result;
	// matSize t_size0 = getSize(data[0]);
	// matSize t_size1 = getSize(data[1]);
	// if(t_size0.cols != t_size1.rows){
	// 	t_result.stateCode = operationError;
	// 	t_result.message = "The matrices are mismatched!";
	// }else{
	// 	t_result.result = data[0] * data[1];
	// }
	// return t_result;
}