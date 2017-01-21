#ifndef PANDAMAT_DIMDISTANCE_H
#define PANDAMAT_DIMDISTANCE_H
#include "operation.h"

class DimDistance: public Operation{
public:
	DimDistance(){};
	DimDistance(int v_par, parameterType* v_types, parameterType v_type);
	void operate();
	exceptions get(mat& v_codes, mat& v_result);
protected:
	double getDistance(rowvec& v_code_i, rowvec& v_code_j);
};
#endif
