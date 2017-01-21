#ifndef PANDAMAT_NORMALIZE_H
#define PANDAMAT_NORMALIZE_H
#include "operation.h"

class Normalize: public Operation{
public:
	Normalize(){};
	Normalize(int v_par, parameterType* v_types, parameterType v_type);
	void operate();
};
#endif
