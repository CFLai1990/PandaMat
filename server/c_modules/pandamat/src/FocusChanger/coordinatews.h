#ifndef PANDAMAT_COORDINATEWS_H
#define PANDAMAT_COORDINATEWS_H
#include "operation.h"
#include "coordinate.h"

class CoordinateWS: public Operation{
public:
	CoordinateWS(){};
	CoordinateWS(int v_par, parameterType* v_types, parameterType v_type);
	void operate();
	void get(mat& v_mat, mat& v_dist, bool v_expand, mat& v_result);
	void subspaceMDS(mat& v_data, mat& v_subvec, bool v_expand, mat& v_result);
private:
	Coordinate oprCoord;
};
#endif
