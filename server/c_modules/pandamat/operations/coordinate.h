#ifndef PANDAMAT_COORDINATE_H
#define PANDAMAT_COORDINATE_H
#include "operation.h"

class Coordinate: public Operation{
public:
	Coordinate(){};
	Coordinate(int v_par, parameterType* v_types, parameterType v_type);
	void operate();
};
#endif
