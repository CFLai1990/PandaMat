#ifndef PANDAMAT_KNNGRAPH_H
#define PANDAMAT_KNNGRAPH_H
#include "operation.h"
#include "flann/flann.hpp"

using namespace flann;

class KNNGraph: public Operation{
public:
	KNNGraph(){};
	KNNGraph(int v_par, parameterType* v_types, parameterType v_type);
	void operate();
	exceptions get(mat& v_data, unsigned int v_k, mat& v_result, string v_type);
protected:
	exceptions unpackMatrix(mat& v_data, matSize& v_size, double* v_result);
	exceptions packMatrix(Matrix<int> v_matrix, mat& v_result);
};
#endif
