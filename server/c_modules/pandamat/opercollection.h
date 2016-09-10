#ifndef PANDAMAT_OPERATIONCOLLECTION_H
#define PANDAMAT_OPERATIONCOLLECTION_H
#include <iostream>
#include <stdlib.h>
#include <string>
#include <node.h>
#include <v8.h>
#include <armadillo>
#include "operation.h"
#include "add.h"
#include "dot.h"
#include "transpose.h"

class OperationCollection{
public:
	OperationResult find(const char * v_operName);
protected:
	Add oprAdd;
	Dot oprDot;
	Transpose oprTrans;
};
#endif
