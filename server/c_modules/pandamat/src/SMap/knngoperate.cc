#include "knngoperate.h"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace boost::filesystem;
using namespace boost::algorithm;

KNNGOperate::KNNGOperate(int v_par, parameterType* v_types, parameterType v_type, string v_dir){
	parameterNum = v_par;
	for(int i = 0; i < v_par; i++){
		parameterTypes[i] = v_types[i];
	}
	data.setResultType(v_type);
	usrDir = v_dir;
	files.insert(pair<string, string>("wordmap", "wordmap.txt"));
	files.insert(pair<string, string>("doc_topic", "model-final.theta"));
	files.insert(pair<string, string>("topic_word", "model-final.phi"));
};

void KNNGOperate::operate(){
	string t_model_name, t_operation, t_target;
	mat t_result = zeros<mat>(1,1);
	bool success0 = data.getData(0, t_model_name);
	bool success1 = data.getData(1, t_operation);
	bool success2 = data.getData(2, t_target);
	if(success0 && success1 && success2){
		exceptions t_state = get(t_model_name, t_operation, t_target, t_result);
		if(t_state != success){
			data.setState(operationError, "Unknown error!");
		}else{
			data.saveResult(t_result);
		}
	}
}

exceptions KNNGOperate::get(string& v_model_name, string& v_operation, string& v_target, mat& v_result){
	string t_dirPath = usrDir + v_model_name + '/';
	path t_dir(t_dirPath.c_str());
	bool t_success = true;
	if(v_operation == "delete"){
		if(exists(t_dir)){
			remove_all(t_dir);
		}
	}else{
		if(!exists(t_dir)){
			t_success = false;
		}else{
			if(v_operation == "get"){
				if(v_target == "wordmap"){
					t_success = getWordMap(t_dirPath, v_result);
				}else{
					t_success = getMatrix(t_dirPath, v_target, v_result);
				}
			}
		}
	}
	if(t_success){
		return success;
	}else{
		return operationError;
	}
}

bool KNNGOperate::getWordMap(string& v_path, mat& v_result){
	string t_path = v_path + files.at("wordmap");
	ifstream t_in(t_path, ios::in|ios::binary);
	if(t_in.is_open()){
		string t_word_num_string;
		unsigned int t_word_num;
		getline(t_in, t_word_num_string);
		t_word_num = stoi(t_word_num_string);
		v_result = zeros<mat>(t_word_num, 2);
		for(unsigned int i = 0; i < t_word_num; i++){
			unsigned int t_num0, t_num1, t_index;
			string t_string;
			getline(t_in, t_string, '_');
			t_num0 = stoi(t_string);
			getline(t_in, t_string, ' ');
			t_num1 = stoi(t_string);
			getline(t_in, t_string);
			t_index = stoi(t_string);
			v_result(t_index, 0) = t_num0;
			v_result(t_index, 1) = t_num1;
		}
		return true;
	}else{
		return false;
	}
}

bool KNNGOperate::getMatrix(string& v_path, string& v_target, mat& v_result){
	if(files.find(v_target) == files.end()){
		return false;
	}
	string t_path = v_path + files.at(v_target);
	ifstream t_in(t_path, ios::in|ios::binary);
	if(t_in.is_open()){
		string t_string;
		while(!t_in.eof()){
			getline(t_in, t_string);
			vector<string> t_num_strings;
			split(t_num_strings, t_string, is_any_of(" "));
			unsigned int t_num_size = t_num_strings.size();
			mat t_row = zeros<mat>(1, t_num_size);
			int t_length = 0;
			for(unsigned int i = 0; i < t_num_size; i++){
				string t_num = t_num_strings[i];
				if(t_num != "\0" && t_num != " "){
					t_row(0, i) = stod(t_num_strings[i]);
					t_length ++;
				}
			}
			if(t_length >0){
				t_row = t_row.cols(0, t_length - 1);
				matSize t_size = getSize(v_result);
				if(t_size.rows == 1 && t_size.cols == 1){
					v_result = zeros<mat>(1, t_length);
					v_result.row(0) = t_row;
				}else{
					v_result = join_cols(v_result, t_row);
				}
			}
		}
		return true;
	}else{
		return false;
	}
}
