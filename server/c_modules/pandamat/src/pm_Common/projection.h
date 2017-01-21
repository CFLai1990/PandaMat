#ifndef PANDAMAT_PROJECTION_H
#define PANDAMAT_PROJECTION_H
#include "operation.h"
#include <algorithm>

using namespace std;

class Projection: public Operation{
public:
	Projection(){};
	Projection(int v_par, parameterType* v_types, parameterType v_type);
	void operate();
	exceptions get(mat& v_dist, string& v_type, unsigned int v_dim, mat& v_result);
};
#endif
