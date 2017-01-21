#ifndef PANDAMAT_SUBSAMPLING_H
#define PANDAMAT_SUBSAMPLING_H
#include <time.h>
#include "pbasics.h"
#include "operation.h"

class Subsampling: public Operation{
public:
	Subsampling(){};
	Subsampling(int v_par, parameterType* v_types, parameterType v_type);
	void operate();
	exceptions get(vec& v_ranges, unsigned int v_dimensions, unsigned int v_sampleNumber, mat& v_result);
protected:
	unsigned long long getProbabilities(unsigned int v_low, unsigned int v_high, unsigned int v_dimensions,
		map<unsigned int, unsigned long long>& v_combNum, map<unsigned int, double>& v_combProb);
	void sampleDimensions(map<unsigned int, unsigned long long>& v_combNum, 
		map<unsigned int, double>& v_combProb, unsigned int v_sampleNumber, vector<unsigned int>& v_results);
	void sampleSubspaces(unsigned int v_dimNumber, vector<unsigned int>& v_dimensions, mat& v_result);
	string sampleCode(unsigned int v_dimensions, unsigned int v_dimNumber);
	void packSamples(vector<string>& v_codes, mat& v_result);
};
#endif
