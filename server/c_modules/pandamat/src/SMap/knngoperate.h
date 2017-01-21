#ifndef PANDAMAT_KNNGOPERATE_H
#define PANDAMAT_KNNGOPERATE_H
#include "operation.h"

class KNNGOperate: public Operation{
public:
	KNNGOperate(int v_par, parameterType* v_types, parameterType v_type, string v_dir);
	void operate();
	exceptions get(string& v_model_name, string& v_operation, string& v_target, mat& v_result);
private:
	map<string, string> files;
	string usrDir;
	bool getWordMap(string& v_path, mat& v_result);
	bool getMatrix(string& v_path, string& v_target, mat& v_result);
};
#endif
