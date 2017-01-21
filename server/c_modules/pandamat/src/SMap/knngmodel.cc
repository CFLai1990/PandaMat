#include "knngmodel.h"
#include <boost/filesystem.hpp>

using namespace boost::filesystem;

KNNGModel::KNNGModel(int v_par, parameterType* v_types, parameterType v_type, string v_lda_dir){
	parameterNum = v_par;
	for(int i = 0; i < v_par; i++){
		parameterTypes[i] = v_types[i];
	}
	data.setResultType(v_type);
	lda_dir = v_lda_dir;
};

KNNGModel::~KNNGModel(){
	// #ifndef MODEL_TEST
	// for(unsigned int i = 0;i < MyModels.size(); i++){
	// 	string t_model_name = MyModels[i];
	// 	string t_dirPath = lda_dir + t_model_name + '/';
	// 	path t_dir(t_dirPath.c_str());
	// 	if(exists(t_dir)){
	// 		remove_all(t_dir);
	// 	}
	// }
	// #endif
}

void KNNGModel::operate(){
	double t_k, t_topic_num;
	string t_model, t_model_name;
	mat t_KNNGs, t_result;
	bool success0 = data.getData(0, t_model);
	bool success1 = data.getData(1, t_KNNGs);
	bool success2 = data.getData(2, t_k);	
	bool success3 = data.getData(3, t_topic_num);
	bool success4 = data.getData(4, t_model_name);
	if(success0 && success1 && success2 && success3 && success4){
		exceptions t_state = get(t_KNNGs, (int)t_k, t_model, (unsigned int)t_topic_num, t_model_name, t_result);
		if(t_state != success){
			data.setState(operationError, "Unknown error!");
		}else{
			data.saveResult(t_result);
		}
	}
}

exceptions KNNGModel::get(mat& v_KNNGs, int v_k, string& v_type, unsigned int v_topic_num, string v_model_name, mat& v_result){
	matSize t_size = getSize(v_KNNGs);
	bool t_success = true;
	// NMFALSFactorizer nmf;
	if(!saveKNNGs(v_KNNGs, v_k, v_model_name)){
		return operationError;
	}
	string new_dir = lda_dir + v_model_name + '/';
	if(v_type == "LDA" || v_type == "lda"){
		LDAParameter t_parameters(t_size.rows, v_topic_num, iter_num, alpha, beta);
		oprLDA.getParameters(t_parameters);
		t_success = oprLDA.run_model(new_dir, lda_filename);
		v_result = zeros<mat>(1,1);
		MyModels.push_back(v_model_name);
	}
	if(t_success){
		return success;
	}else{
		return operationError;
	}
}

bool KNNGModel::saveKNNGs(mat& v_KNNGs, int v_k, string v_model_name){
	string t_dirPath = lda_dir + v_model_name + '/';
	path t_dir(t_dirPath.c_str());
	if(!exists(t_dir)){
		if(!create_directory(t_dir)){
			return false;
		}
	}
	string t_filePath = t_dirPath + lda_filename;
	path t_file(t_filePath);
	if(exists(t_file)){
		remove(t_file);
	}
	ofstream out(t_filePath);
	matSize t_size = getSize(v_KNNGs);
	if(out.is_open()){
		// cout << "Saving KNNGraphs success!" << endl;
		out << t_size.rows << "\n";
		for(unsigned int i = 0; i < t_size.rows; i++){
			for(unsigned int j = 0; j < t_size.cols; j++){
				unsigned int p1 = (unsigned int)(j)/(unsigned int)v_k;
				unsigned int p2 = v_KNNGs(i,j);
				string t_word;
				if(p1 < p2){
					t_word = num2str(p1) + "_" + num2str(p2);
				}else{
					t_word = num2str(p2) + "_" + num2str(p1);
				}
				out << t_word.c_str() << " ";
			}
			out << endl;
		}
		return true;
	}else{		
		// cout << "Saving KNNGraphs fail!" << endl;
		return false;
	}
}
