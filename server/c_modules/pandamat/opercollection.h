#ifndef PANDAMAT_OPERATIONCOLLECTION_H
#define PANDAMAT_OPERATIONCOLLECTION_H
#include <iostream>
#include <stdlib.h>
#include <string>
#include <node.h>
#include <v8.h>
#include <armadillo>
#include <map>
#include <boost/filesystem.hpp>
#include "pbasics.h"
#include "operation.h"
#include "add.h"
#include "dot.h"
#include "multiply.h"
#include "transpose.h"
#include "normalize.h"
#include "sqrtdist.h"
#include "coordinate.h"
#include "coordinatews.h"
#include "coordstransform.h"
#include "getcoordinates.h"
#include "projection.h"
#include "mds.h"
#include "kmeans.h"
#include "subspace.h"
#include "subsampling.h"
#include "dimdistance.h"
#include "knngraph.h"
#include "knngraphs.h"
#include "knngdistance.h"
#include "knngmodel.h"
#include "knngoperate.h"

using namespace boost::filesystem;

class OperationCollection{
public:
	OperationCollection();
	void getUserDataPath(string v_path);
	OperationResult create(unsigned int v_id, const char * v_operName);
	void destroy(unsigned int v_id);
protected:
	map<unsigned int, string> oprCommands;
	map<unsigned int, Add> oprAdd;
	map<unsigned int, Dot> oprDot;
	map<unsigned int, Multiply> oprMultiply;
	map<unsigned int, Transpose> oprTrans;
	map<unsigned int, Normalize> oprNorm;
	map<unsigned int, SqrtDist> oprDist;
	map<unsigned int, Coordinate> oprCoord;
	map<unsigned int, CoordinateWS> oprCoordWS;
	map<unsigned int, CoordinateTransform> oprCoordsTF;
	map<unsigned int, Projection> oprProj;
	map<unsigned int, Mds> oprMds;
	map<unsigned int, Kmeans> oprKmeans;
	map<unsigned int, Subspace> oprSubspace;
	map<unsigned int, Subsampling> oprSubsampling;
	map<unsigned int, KNNGraph> oprKNNG;
	map<unsigned int, KNNGraphs> oprKNNGS;
	map<unsigned int, KNNGModel> oprKNNGModel;
	map<unsigned int, KNNGOperate> oprKNNGOperate;
	map<unsigned int, KNNGDistance> oprKNNGDist;
	map<unsigned int, DimDistance> oprDimDist;
	map<unsigned int, GetCoordinates> oprGetCoord;
private:
	string userDataPath;
};
#endif
