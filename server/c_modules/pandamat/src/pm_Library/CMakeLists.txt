CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(pm_library)

SET(CMAKE_CXX_COMPILER "gcc")

IF(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -Wall -std=gnu++0x")
ENDIF()
  
SET(SRCS  
    basic/operation.cc
    basic/operationdata.cc  
    basic/pbasics.cc
    lda/lda.cpp
)

SET (LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../lib)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR} 
	/opt/node-v4.2.4/include/node/ 
	/opt/armadillo-7.400.2-gcc6.2.0/include/
	/var/home/chufan.lai/library/LDA/
)

ADD_LIBRARY(pm_library SHARED ${SRCS})

SET_TARGET_PROPERTIES(pm_library PROPERTIES VERSION 1.0 SOVERSION 1)