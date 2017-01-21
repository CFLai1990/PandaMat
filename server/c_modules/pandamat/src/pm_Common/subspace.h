#ifndef PANDAMAT_SUBSPACE_H
#define PANDAMAT_SUBSPACE_H
#include "operation.h"

class Subspace: public Operation{
public:
	Subspace(){};
	Subspace(int v_par, parameterType* v_types, parameterType v_type);
	void operate();
	exceptions get(mat& v_data, vec& v_sub, mat& v_subdata);
};
#endif
