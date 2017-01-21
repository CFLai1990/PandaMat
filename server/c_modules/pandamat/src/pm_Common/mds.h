#ifndef PANDAMAT_MDS_H
#define PANDAMAT_MDS_H
#include "operation.h"
#include "coordinate.h"

class Mds: public Operation{
public:
	Mds(){};
	Mds(int v_par, parameterType* v_types, parameterType v_type);
	void operate();
	void get(mat& v_mat, mat& v_result);
protected:
	Coordinate oprCoord;
};
#endif
