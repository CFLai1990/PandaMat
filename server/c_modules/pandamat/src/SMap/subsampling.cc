#include "subsampling.h"

unsigned int prand(unsigned int v_low, unsigned int v_high){
	unsigned int t_div = v_high - v_low + 1;
	return (unsigned int)(rand()%t_div + v_low);
}

double prand(){
	return (double)(rand()/((double)RAND_MAX + 1));
}

unsigned long long combNum(unsigned long long n, unsigned long long k) {
    unsigned long long r = 1;
    for (unsigned long long d = 1; d <= k; ++d) {
        r *= n--;
        r /= d;
    }
    return r;
}

Subsampling::Subsampling(int v_par, parameterType* v_types, parameterType v_type){
	parameterNum = v_par;
	for(int i = 0; i < v_par; i++){
		parameterTypes[i] = v_types[i];
	}
	data.setResultType(v_type);
};

void Subsampling::operate(){
	vec t_ranges;double t_dimNumber, t_sampleNumber;
	mat t_result;
	bool success0 = data.getData(0, t_ranges);
	bool success1 = data.getData(1, t_dimNumber);
	bool success2 = data.getData(2, t_sampleNumber);
	if(success0 && success1 && success2){
		exceptions t_state = get(t_ranges, (unsigned int)t_dimNumber, (unsigned int)t_sampleNumber, t_result);
		if(t_state != success){
			data.setState(operationError, "Unknown error!");
		}else{
			data.saveResult(t_result);
		}
	}
}

exceptions Subsampling::get(vec& v_ranges, unsigned int v_dimNumber, unsigned int v_sampleNumber, mat& v_result){
	if(	(v_ranges(0) <=0) || (v_ranges(1) <=0)
		|| (v_ranges(0) > v_ranges(1))
		|| (v_dimNumber < (unsigned int) (v_ranges(1))) ){
		return operationError;
	}else{
		unsigned int t_low = (unsigned int)v_ranges(0), t_high = (unsigned int)v_ranges(1);
		srand((int)time(0));
		vector<unsigned int> t_dimensions;
		map<unsigned int, unsigned long long> t_combNum;
		map<unsigned int, double> t_combProb;
		unsigned long long t_sum = getProbabilities(t_low, t_high, v_dimNumber, t_combNum, t_combProb);
		if((unsigned long long)(v_sampleNumber) > t_sum){
			v_sampleNumber = (unsigned int)(t_sum);
		}
		sampleDimensions(t_combNum, t_combProb, v_sampleNumber, t_dimensions);
		sampleSubspaces(v_dimNumber, t_dimensions, v_result);
		return success;
	}
}

unsigned long long Subsampling::getProbabilities(unsigned int v_low, unsigned int v_high, unsigned int v_dimensions,
	map<unsigned int, unsigned long long>& v_combNum,
	map<unsigned int, double>& v_combProb){
		unsigned long long t_sum = 0;
		for(unsigned int i = v_low; i <= v_high; i++){
			unsigned long long t_combNum_i = combNum(v_dimensions, i);
			v_combNum.insert(pair<unsigned int, unsigned long long>(i, t_combNum_i));
			t_sum += t_combNum_i;
		}
		double t_prob = 0;
		for(unsigned int i = v_low; i <= v_high; i++){
			unsigned long long t_combNum_i = v_combNum.at(i);
			double t_prob_i = (double)(t_combNum_i) / (double)(t_sum);
			t_prob += t_prob_i;
			if(i == v_high){
				t_prob = 1;
			}
			v_combProb.insert(pair<unsigned int, double>(i, t_prob));
		}
		return t_sum;
}

void Subsampling::sampleDimensions(map<unsigned int, unsigned long long>& v_combNum,
	map<unsigned int, double>& v_combProb, unsigned int v_sampleNumber, vector<unsigned int>& v_results){
	map<unsigned int, int> t_combNum;
	map<unsigned int, unsigned long long>::iterator it;
	for(it = v_combNum.begin(); it != v_combNum.end(); it++){
		t_combNum.insert(pair<unsigned int, unsigned int>(it -> first, 0));
	}
	unsigned int t_count = 0;
	while(t_count < v_sampleNumber){
		double t_rand = prand(), t_low = 0;
		unsigned int t_dimension;
		map<unsigned int, double>::iterator it;
		for(it = v_combProb.begin(); it != v_combProb.end(); it++){
			double t_high = it -> second;
			if(t_rand >= t_low && t_rand <= t_high){
				t_dimension = it -> first;
				break;
			}
			t_low = t_high;
		}
		unsigned int t_number = t_combNum.at(t_dimension), t_top = (unsigned int)(v_combNum.at(t_dimension));
		if(t_number < t_top){
			v_results.push_back(t_dimension);
			t_count++;
			t_combNum.at(t_dimension) = t_number + 1;
		}
	}
}

void Subsampling::sampleSubspaces(unsigned int v_dimNumber, vector<unsigned int>& v_dimensions, mat& v_result){
	vector<string> t_codes;
	vector<unsigned int>::iterator it;
	for(it = v_dimensions.begin(); it != v_dimensions.end(); it++){
		bool t_next = false;
		while(!t_next){
			string t_code = sampleCode(*it, v_dimNumber);
 			if(find(t_codes.begin(), t_codes.end(), t_code) == t_codes.end()){
				t_codes.push_back(t_code);
				t_next = true;
			}
		}
	}
	packSamples(t_codes, v_result);
}

string Subsampling::sampleCode(unsigned int v_dimensions, unsigned int v_dimNumber){
	string t_code("");
	vector<unsigned int> t_positions;
	for(unsigned int i = 0; i < v_dimNumber; i++){
		t_code += "0";
		t_positions.push_back(i);
	}
	for(unsigned int i = v_dimNumber - 1;i < v_dimNumber && i >= v_dimNumber - v_dimensions; i--){
		unsigned int t_rand;
		if(i > 0){
			t_rand = prand(0, i);
		}else{
			t_rand = 0;
		}
		unsigned int t_pos = t_positions[t_rand];
		t_code.replace(t_pos,1,"1");
		vector<unsigned int>::iterator it = find(t_positions.begin(), t_positions.end(), t_pos);
		if(it != t_positions.end()){
			t_positions.erase(it);
		}
	}
	return t_code;
}

void Subsampling::packSamples(vector<string>& v_codes, mat& v_result){
	unsigned int t_length = -1, t_samples = v_codes.size();
	for(unsigned int i = 0; i < t_samples; i++){
		if(i == 0){
			t_length = v_codes[i].length();
			v_result = zeros<mat>(t_samples, t_length);
		}
		string t_code = v_codes[i];
		for(unsigned int j = 0; j < t_length; j++){
			if(t_code[j] == '1'){
				v_result(i, j) = 1;
			}
		}
	}
}
