#include "opercollection.h"

OperationCollection::OperationCollection(){
}

void OperationCollection::getUserDataPath(string v_path){
	userDataPath = v_path;
}

OperationResult OperationCollection::create(unsigned int v_id, const char * v_operName){
	string t_name(v_operName);
	OperationResult t_result;
	bool found = false;
#define operation_handle(v_name, v_opr, v_oprType, v_return, v_parm_num, ...)									\
	if(t_name == v_name){																						\
		found = true;																							\
		parameterType t_types[v_parm_num] = {__VA_ARGS__};														\
		parameterType t_type = v_return;																		\
		v_opr.insert(pair<unsigned int, v_oprType>(v_id, v_oprType(v_parm_num, t_types, t_type)));				\
		t_result.operation = &(v_opr.at(v_id));																	\
	}																											\

#define operationWithData_handle(v_name, v_opr, v_oprType, v_return, v_parm_num, ...)							\
	if(t_name == v_name){																						\
		found = true;																							\
		parameterType t_types[v_parm_num] = {__VA_ARGS__};														\
		parameterType t_type = v_return;																		\
		v_opr.insert(pair<unsigned int, v_oprType>(v_id, v_oprType(v_parm_num, t_types, t_type, userDataPath)));\
		t_result.operation = &(v_opr.at(v_id));																	\
	}																											\

	operation_handle("add", oprAdd, Add, p_matrix, 2, p_matrix, p_matrix);
	operation_handle("dot", oprDot, Dot, p_matrix, 2, p_matrix, p_matrix);
	operation_handle("multiply", oprMultiply, Multiply, p_matrix, 2, p_matrix, p_value);
	operation_handle("trans", oprTrans, Transpose, p_matrix, 1, p_matrix);
	operation_handle("normalize", oprNorm, Normalize, p_matrix, 1, p_matrix);
	operation_handle("sqrtDist", oprDist, SqrtDist, p_matrix, 1, p_matrix);
	operation_handle("coordinate", oprCoord, Coordinate, p_matrix, 2, p_matrix, p_bool);
	operation_handle("coordinatews", oprCoordWS, CoordinateWS, p_matrix, 3, p_matrix, p_matrix, p_bool);
	operation_handle("coordstransform", oprCoordsTF, CoordinateTransform, p_matrix, 3, p_matrix, p_matrix, p_value);
	operation_handle("getcoordinates", oprGetCoord, GetCoordinates, p_matrix, 5, p_vector, p_string, p_vector, p_matrix, p_bool);
	operation_handle("projection", oprProj, Projection, p_matrix, 3, p_matrix, p_string, p_value);
	operation_handle("mds", oprMds, Mds, p_matrix, 1, p_matrix);
	operation_handle("kmeans", oprKmeans, Kmeans, p_vector, 2, p_matrix, p_value);
	operation_handle("subspace", oprSubspace, Subspace, p_matrix, 2, p_matrix, p_vector);
	operation_handle("subsampling", oprSubsampling, Subsampling, p_matrix, 3, p_vector, p_value, p_value);
	operation_handle("KNNGraph", oprKNNG, KNNGraph, p_matrix, 3, p_matrix, p_value, p_string);
	operation_handle("KNNGraphs", oprKNNGS, KNNGraphs, p_matrix, 4, p_matrix, p_matrix, p_value, p_string);
	operation_handle("KNNGDistance", oprKNNGDist, KNNGDistance, p_matrix, 2, p_matrix, p_value);
	operation_handle("dimDistance", oprDimDist, DimDistance, p_matrix, 1, p_matrix);
	operationWithData_handle("KNNGModel", oprKNNGModel, KNNGModel, p_matrix, 5, p_string, p_matrix, p_value, p_value, p_string);
	operationWithData_handle("KNNGOperate", oprKNNGOperate, KNNGOperate, p_matrix, 3, p_string, p_string, p_string);
#undef operation_handle
#undef operationWithData_handle
	// if(t_name == "add"){
	// 	found = true;
	// 	parameterType t_types[2] = {p_matrix, p_matrix};
	// 	parameterType t_type = p_matrix;
	// 	oprAdd.insert(pair<unsigned int, Add>(v_id, Add(2, t_types, t_type)));
	// 	t_result.operation = &(oprAdd.at(v_id));
	// }
	// if(t_name == "dot"){
	// 	found = true;
	// 	parameterType t_types[2] = {p_matrix, p_matrix};
	// 	parameterType t_type = p_matrix;
	// 	oprDot.insert(pair<unsigned int, Dot>(v_id, Dot(2, t_types, t_type)));
	// 	t_result.operation = &(oprDot.at(v_id));
	// }
	// if(t_name == "multiply"){
	// 	found = true;
	// 	parameterType t_types[2] = {p_matrix, p_value};
	// 	parameterType t_type = p_matrix;
	// 	oprMultiply.insert(pair<unsigned int, Multiply>(v_id, Multiply(2, t_types, t_type)));
	// 	t_result.operation = &(oprMultiply.at(v_id));
	// }
	// if(t_name == "trans"){
	// 	found = true;
	// 	parameterType t_types[1] = {p_matrix};
	// 	parameterType t_type = p_matrix;
	// 	oprTrans.insert(pair<unsigned int, Transpose>(v_id, Transpose(1, t_types, t_type)));
	// 	t_result.operation = &(oprTrans.at(v_id));
	// }
	// if(t_name == "normalize"){
	// 	found = true;
	// 	parameterType t_types[1] = {p_matrix};
	// 	parameterType t_type = p_matrix;
	// 	oprNorm.insert(pair<unsigned int, Normalize>(v_id, Normalize(1, t_types, t_type)));
	// 	t_result.operation = &(oprNorm.at(v_id));
	// }
	// if(t_name == "sqrtDist"){
	// 	found = true;
	// 	parameterType t_types[1] = {p_matrix};
	// 	parameterType t_type = p_matrix;
	// 	oprDist.insert(pair<unsigned int, SqrtDist>(v_id, SqrtDist(1, t_types, t_type)));
	// 	t_result.operation = &(oprDist.at(v_id));
	// }
	// if(t_name == "coordinate"){
	// 	found = true;
	// 	parameterType t_types[2] = {p_matrix, p_bool};
	// 	parameterType t_type = p_matrix;
	// 	oprCoord.insert(pair<unsigned int, Coordinate>(v_id, Coordinate(2, t_types, t_type)));
	// 	t_result.operation = &(oprCoord.at(v_id));
	// }
	// if(t_name == "coordinatews"){
	// 	found = true;
	// 	parameterType t_types[3] = {p_matrix, p_matrix, p_bool};
	// 	parameterType t_type = p_matrix;
	// 	oprCoordWS.insert(pair<unsigned int, CoordinateWS>(v_id, CoordinateWS(3, t_types, t_type)));
	// 	t_result.operation = &(oprCoordWS.at(v_id));
	// }
	// if(t_name == "getcoordinates"){
	// 	found = true;
	// 	parameterType t_types[5] = {p_vector, p_string, p_vector, p_matrix, p_bool};
	// 	parameterType t_type = p_matrix;
	// 	oprGetCoord.insert(pair<unsigned int, GetCoordinates>(v_id, GetCoordinates(5, t_types, t_type)));
	// 	t_result.operation = &(oprGetCoord.at(v_id));
	// }
	// if(t_name == "projection"){
	// 	found = true;
	// 	parameterType t_types[3] = {p_matrix, p_string, p_value};
	// 	parameterType t_type = p_matrix;
	// 	oprProj.insert(pair<unsigned int, Projection>(v_id, Projection(3, t_types, t_type)));
	// 	t_result.operation = &(oprProj.at(v_id));
	// }
	// if(t_name == "mds"){
	// 	found = true;
	// 	parameterType t_types[1] = {p_matrix};
	// 	parameterType t_type = p_matrix;
	// 	oprMds.insert(pair<unsigned int, Mds>(v_id, Mds(1, t_types, t_type)));
	// 	t_result.operation = &(oprMds.at(v_id));
	// }
	// if(t_name == "kmeans"){
	// 	found = true;
	// 	parameterType t_types[2] = {p_matrix, p_value};
	// 	parameterType t_type = p_vector;
	// 	oprKmeans.insert(pair<unsigned int, Kmeans>(v_id, Kmeans(2, t_types, t_type)));
	// 	t_result.operation = &(oprKmeans.at(v_id));
	// }
	// if(t_name == "subspace"){
	// 	found = true;
	// 	parameterType t_types[2] = {p_matrix, p_vector};
	// 	parameterType t_type = p_matrix;
	// 	oprSubspace.insert(pair<unsigned int, Subspace>(v_id, Subspace(2, t_types, t_type)));
	// 	t_result.operation = &(oprSubspace.at(v_id));
	// }
	// if(t_name == "subsampling"){
	// 	found = true;
	// 	parameterType t_types[3] = {p_vector, p_value, p_value};
	// 	parameterType t_type = p_matrix;
	// 	oprSubsampling.insert(pair<unsigned int, Subsampling>(v_id, Subsampling(3, t_types, t_type)));
	// 	t_result.operation = &(oprSubsampling.at(v_id));
	// }
	// if(t_name == "KNNGraph"){
	// 	found = true;
	// 	parameterType t_types[3] = {p_matrix, p_value, p_string};
	// 	parameterType t_type = p_matrix;
	// 	oprKNNG.insert(pair<unsigned int, KNNGraph>(v_id, KNNGraph(3, t_types, t_type)));
	// 	t_result.operation = &(oprKNNG.at(v_id));
	// }
	// if(t_name == "KNNGraphs"){
	// 	found = true;
	// 	parameterType t_types[4] = {p_matrix, p_matrix, p_value, p_string};
	// 	parameterType t_type = p_matrix;
	// 	oprKNNGS.insert(pair<unsigned int, KNNGraphs>(v_id, KNNGraphs(4, t_types, t_type)));
	// 	t_result.operation = &(oprKNNGS.at(v_id));
	// }
	// if(t_name == "KNNGModel"){
	// 	found = true;
	// 	parameterType t_types[5] = {p_string, p_matrix, p_value, p_value, p_string};
	// 	parameterType t_type = p_matrix;
	// 	oprKNNGModel.insert(pair<unsigned int, KNNGModel>(v_id, KNNGModel(5, t_types, t_type, userDataPath)));
	// 	t_result.operation = &(oprKNNGModel.at(v_id));
	// }
	// if(t_name == "KNNGOperate"){
	// 	found = true;
	// 	parameterType t_types[3] = {p_string, p_string, p_string};
	// 	parameterType t_type = p_matrix;
	// 	oprKNNGOperate.insert(pair<unsigned int, KNNGOperate>(v_id, KNNGOperate(3, t_types, t_type, userDataPath)));
	// 	t_result.operation = &(oprKNNGOperate.at(v_id));
	// }
	// if(t_name == "KNNGDistance"){
	// 	found = true;
	// 	parameterType t_types[2] = {p_matrix, p_value};
	// 	parameterType t_type = p_matrix;
	// 	oprKNNGDist.insert(pair<unsigned int, KNNGDistance>(v_id, KNNGDistance(2, t_types, t_type)));
	// 	t_result.operation = &(oprKNNGDist.at(v_id));
	// }
	// if(t_name == "dimDistance"){
	// 	found = true;
	// 	parameterType t_types[1] = {p_matrix};
	// 	parameterType t_type = p_matrix;
	// 	oprDimDist.insert(pair<unsigned int, DimDistance>(v_id, DimDistance(1, t_types, t_type)));
	// 	t_result.operation = &(oprDimDist.at(v_id));
	// }
	if(!found){
		t_result.stateCode = noCommandError;
		t_result.stateMessage = "Command ";
		t_result.stateMessage += t_name;
		t_result.stateMessage += " not found!";
	}else{
		oprCommands.insert(pair<unsigned int, string>(v_id, t_name));
	}
	return t_result;
}

void OperationCollection::destroy(unsigned int v_id){
	string t_command = oprCommands.at(v_id);
#define destroy_handle(v_name, v_opr)			\
	if(t_command == v_name){					\
		v_opr.erase(v_id);						\
	}											\

	destroy_handle("add", oprAdd);
	destroy_handle("dot", oprDot);
	destroy_handle("multiply", oprMultiply);
	destroy_handle("trans", oprTrans);
	destroy_handle("normalize", oprNorm);
	destroy_handle("sqrtDist", oprDist);
	destroy_handle("coordinate", oprCoord);
	destroy_handle("coordinatews", oprCoordWS);
	destroy_handle("coordstransform", oprCoordsTF);
	destroy_handle("getcoordinates", oprGetCoord);
	destroy_handle("projection", oprProj);
	destroy_handle("mds", oprMds);
	destroy_handle("kmeans", oprKmeans);
	destroy_handle("subspace", oprSubspace);
	destroy_handle("subsampling", oprSubsampling);
	destroy_handle("KNNGraph", oprKNNG);
	destroy_handle("KNNGraphs", oprKNNGS);
	destroy_handle("KNNGDistance", oprKNNGDist);
	destroy_handle("dimDistance", oprDimDist);
	destroy_handle("KNNGModel", oprKNNGModel);
	destroy_handle("KNNGOperate", oprKNNGOperate);
#undef destroy_handle

	// if(t_command == "add"){
	// 	oprAdd.erase(v_id);
	// }
	// if(t_command == "dot"){
	// 	oprDot.erase(v_id);
	// }
	// if(t_command == "multiply"){
	// 	oprMultiply.erase(v_id);
	// }
	// if(t_command == "trans"){
	// 	oprTrans.erase(v_id);
	// }
	// if(t_command == "normalize"){
	// 	oprNorm.erase(v_id);
	// }
	// if(t_command == "sqrtDist"){
	// 	oprDist.erase(v_id);
	// }
	// if(t_command == "coordinate"){
	// 	oprCoord.erase(v_id);
	// }
	// if(t_command == "coordinatews"){
	// 	oprCoordWS.erase(v_id);
	// }
	// if(t_command == "getcoordinates"){
	// 	oprGetCoord.erase(v_id);
	// }
	// if(t_command == "projection"){
	// 	oprProj.erase(v_id);
	// }
	// if(t_command == "mds"){
	// 	oprMds.erase(v_id);
	// }
	// if(t_command == "kmeans"){
	// 	oprKmeans.erase(v_id);
	// }
	// if(t_command == "subspace"){
	// 	oprSubspace.erase(v_id);
	// }
	// if(t_command == "subsampling"){
	// 	oprSubsampling.erase(v_id);
	// }
	// if(t_command == "KNNGraph"){
	// 	oprKNNG.erase(v_id);
	// }
	// if(t_command == "KNNGraphs"){
	// 	oprKNNGS.erase(v_id);
	// }
	// if(t_command == "KNNGModel"){
	// 	oprKNNGModel.erase(v_id);
	// }
	// if(t_command == "KNNGOperate"){
	// 	oprKNNGOperate.erase(v_id);
	// }
	// if(t_command == "KNNGDistance"){
	// 	oprKNNGDist.erase(v_id);
	// }
	// if(t_command == "dimDistance"){
	// 	oprDimDist.erase(v_id);
	// }
	oprCommands.erase(v_id);
}