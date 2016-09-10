#ifndef PANDAMAT_ADD_H
#define PANDAMAT_ADD_H
#include "operation.h"

class Add: public Operation{
public:
	Add(){};
	Add(int v_par, parameterType* v_types);
	OperationResult operate();
};
#endif
