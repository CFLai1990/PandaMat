#ifndef PANDAMAT_KMEANS_H
#define PANDAMAT_KMEANS_H
#include "operation.h"

class Kmeans: public Operation{
public:
	Kmeans(){};
	Kmeans(int v_par, parameterType* v_types, parameterType v_type);
	void operate();
	bool get(mat& v_mat, unsigned int v_k, vec& v_result);
	int assign(vec v_vec, mat& v_mat, int v_cols);
};
#endif
