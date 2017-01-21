#ifndef PANDAMAT_SQRTDIST_H
#define PANDAMAT_SQRTDIST_H
#include "operation.h"
#include <omp.h>
#include <time.h>

class SqrtDist: public Operation{
public:
	SqrtDist(){};
	SqrtDist(int v_par, parameterType* v_types, parameterType v_type);
	void operate();
	exceptions get(mat& v_mat, mat& v_result);
};
#endif
