#ifndef PANDAMAT_COORDINATE_H
#define PANDAMAT_COORDINATE_H
#include "operation.h"

class Coordinate: public Operation{
public:
	Coordinate(){};
	Coordinate(int v_par, parameterType* v_types, parameterType v_type);
	void operate();
	void get(mat& v_mat, bool v_expand, mat& v_result);
};
#endif
