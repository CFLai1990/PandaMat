CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(pm_common)

SET(CMAKE_CXX_COMPILER "gcc")

IF(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -Wall -std=gnu++0x")
ENDIF()
  
SET(SRCS  
	add.cc
	coordinate.cc
	coordstransform.cc
	dot.cc
	multiply.cc
	kmeans.cc
	knngraph.cc
	mds.cc
	normalize.cc
	projection.cc
	sqrtdist.cc
	subspace.cc
	transpose.cc
)

SET (LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../lib)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../pm_Library/basic/
	/opt/node-v4.2.4/include/node/ 
	/opt/armadillo-7.400.2-gcc6.2.0/include/
	/var/home/chufan.lai/library/KNN/flann-1.8.4-src/src/cpp/
	/var/home/chufan.lai/eigen3.2.10/
	/var/home/chufan.lai/library/Projection/tapkee/include/
)

ADD_LIBRARY(pm_common SHARED ${SRCS})

SET_TARGET_PROPERTIES(pm_common PROPERTIES VERSION 1.0 SOVERSION 1)