#ifndef PANDAMAT_PBASICS_H
#define PANDAMAT_PBASICS_H
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include "math.h"

using namespace std;

#ifndef pDelta
#define pDelta 1e-13
#endif

enum exceptions{
	success = -1,
	typeError = 0,
	operationError = 1,
	parameterError = 2,
	noCommandError = 3,
	nanError = 4,
	unkownError = 10,
};

enum parameterType{
	p_matrix = 0,
	p_value = 1,
	p_string = 2,
	p_bool = 3,
	p_vector = 4,
	p_unkown = 100,
	p_missing = 1000,
};

template<typename T>
string num2str(const T val){
    ostringstream oss;
    oss << val;
    return oss.str() ;
}

template<typename T1, typename T2>
void copyArray(T1& arr1, T2& arr2,
	unsigned int n_rows, unsigned int n_cols){
	for(unsigned int i = 0; i < n_rows; i++){
		for(unsigned int j = 0; j < n_cols; j++){
			arr2(i,j) = arr1(i,j);
		}
	}
}

template<typename TVec>
void printVector(TVec& v_vec){
	for(unsigned int i = 0; i < v_vec.n_elem; i++){
		cout << v_vec(i) << " ";
	}
	cout << endl;
}

template<typename TMat>
void printMatrix(TMat& v_arr){
	for(unsigned int i = 0; i < v_arr.n_rows; i++){
		for(unsigned int j = 0; j < v_arr.n_cols; j++){
			cout << v_arr(i,j) << " ";
		}
		cout << endl;
	}
	cout << endl;
}
#endif