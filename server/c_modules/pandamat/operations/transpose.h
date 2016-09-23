#ifndef PANDAMAT_TRANSPOSE_H
#define PANDAMAT_TRANSPOSE_H
#include "operation.h"

class Transpose: public Operation{
public:
	Transpose(){};
	Transpose(int v_par, parameterType* v_types, parameterType v_type);
	void operate();
};
#endif
