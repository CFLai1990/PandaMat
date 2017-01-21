#ifndef PANDAMAT_KNNGRAPHS_H
#define PANDAMAT_KNNGRAPHS_H
#include "operation.h"
#include "subspace.h"
#include "sqrtdist.h"
#include "knngraph.h"

class KNNGraphs: public Operation{
public:
	KNNGraphs(){};
	KNNGraphs(int v_par, parameterType* v_types, parameterType v_type);
	void operate();
	exceptions get(mat& v_data, mat& v_codes, unsigned int v_k, mat& v_result, string v_type);
private:
	Subspace oprSub;
	SqrtDist oprDist;
	KNNGraph oprKNNG;
};
#endif
