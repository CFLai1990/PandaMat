#ifndef PANDAMAT_KNNGDISTANCE_H
#define PANDAMAT_KNNGDISTANCE_H
#include "operation.h"

class KNNGDistance: public Operation{
public:
	KNNGDistance(){};
	KNNGDistance(int v_par, parameterType* v_types, parameterType v_type);
	void operate();
	exceptions get(mat& v_graphs, const double& v_k, mat& v_result);
protected:
	double getDistance(rowvec& v_graph_i, rowvec& v_graph_j, const double& v_k);
	double compare(vec& v_nn_i, vec& v_nn_j, const unsigned int& v_k);
};
#endif
