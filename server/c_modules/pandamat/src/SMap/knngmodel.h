#ifndef PANDAMAT_KNNGMODEL_H
#define PANDAMAT_KNNGMODEL_H
#include "operation.h"
#include "lda/lda.h"
// #include <mlpack/core.hpp>
// #include <mlpack/methods/amf/amf.hpp>

// using namespace mlpack::amf;
// #define MODEL_TEST

class KNNGModel: public Operation{
public:
	KNNGModel();
	~KNNGModel();
	KNNGModel(int v_par, parameterType* v_types, parameterType v_type, string v_lda_dir);
	void operate();
	exceptions get(mat& v_KNNGs, int v_k, string& v_type, unsigned int v_topic_num, string v_model_name, mat& v_result);
	bool saveKNNGs(mat& v_KNNGs, int v_k, string v_model_name);
private:
	vector<string> MyModels;
	MyLDA oprLDA;
	const int iter_num = 200;
	const float alpha = -1;
	const float beta = 0.1;
	string lda_dir;
	const string lda_filename = "KNNGs.dat";
};
#endif
