#ifndef PANDAMAT_MULTIPLY_H
#define PANDAMAT_MULTIPLY_H
#include "operation.h"

class Multiply: public Operation{
public:
	Multiply(){};
	Multiply(int v_par, parameterType* v_types, parameterType v_type);
	void operate();
	exceptions get(mat& v_mat, double& v_factor, mat& v_result);
};
#endif
