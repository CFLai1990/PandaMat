CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(pm_focuschanger)

SET(CMAKE_CXX_COMPILER "gcc")

IF(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -Wall -std=gnu++0x")
ENDIF()
  
SET(SRCS  
	coordinatews.cc
	getcoordinates.cc
)

SET (LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../lib)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../pm_Library/basic/
	${CMAKE_CURRENT_SOURCE_DIR}/../pm_Common/
	/opt/node-v4.2.4/include/node/ 
	/opt/armadillo-7.400.2-gcc6.2.0/include/
	/var/home/chufan.lai/eigen3.2.10/
)

ADD_LIBRARY(pm_focuschanger SHARED ${SRCS})

SET_TARGET_PROPERTIES(pm_focuschanger PROPERTIES VERSION 1.0 SOVERSION 1)