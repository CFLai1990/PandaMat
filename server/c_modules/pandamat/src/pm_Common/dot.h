#ifndef PANDAMAT_DOT_H
#define PANDAMAT_DOT_H
#include "operation.h"

class Dot: public Operation{
public:
	Dot(){};
	Dot(int v_par, parameterType* v_types, parameterType v_type);
	void operate();
};
#endif
