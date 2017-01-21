#ifndef PANDAMAT_COORDSTRANSFORM_H
#define PANDAMAT_COORDSTRANSFORM_H
#include <math.h>
#include "operation.h"

class CoordinateTransform: public Operation{
public:
	CoordinateTransform(){};
	CoordinateTransform(int v_par, parameterType* v_types, parameterType v_type);
	void operate();
	exceptions get(mat& v_mat1, mat& v_mat2, unsigned int v_frames, mat& v_result);
};
#endif
