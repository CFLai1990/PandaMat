#ifndef PANDAMAT_OPERATIONCOLLECTION_H
#define PANDAMAT_OPERATIONCOLLECTION_H
#include <iostream>
#include <stdlib.h>
#include <string>
#include <node.h>
#include <v8.h>
#include <armadillo>
#include <map>
#include "pbasics.h"
#include "operation.h"
#include "add.h"
#include "dot.h"
#include "transpose.h"
#include "normalize.h"
#include "sqrtdist.h"

class OperationCollection{
public:
	OperationCollection();
	OperationResult create(unsigned int v_id, const char * v_operName);
	void destroy(unsigned int v_id);
protected:
	map<unsigned int, string> oprCommands;
	map<unsigned int, Add> oprAdd;
	map<unsigned int, Dot> oprDot;
	map<unsigned int, Transpose> oprTrans;
	map<unsigned int, Normalize> oprNorm;
	map<unsigned int, SqrtDist> oprDist;
};
#endif
