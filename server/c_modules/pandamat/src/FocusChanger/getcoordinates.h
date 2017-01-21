#ifndef PANDAMAT_GETCOORDINATES_H
#define PANDAMAT_GETCOORDINATES_H
#include "operation.h"
#include "subspace.h"
#include "coordinatews.h"

class GetCoordinates: public Operation{
public:
	GetCoordinates(){};
	GetCoordinates(int v_par, parameterType* v_types, parameterType v_type);
	void operate();
	exceptions get(mat& v_sub, vec& v_focus, string& v_opti, bool v_centroid, mat& v_result);
private:
	exceptions focus(mat& v_data, vec& v_focus, mat& v_result, bool v_sign);
	Subspace oprSubspace;
	CoordinateWS oprCoordWS;
};
#endif
