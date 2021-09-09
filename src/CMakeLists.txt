include_directories(${PARADISEO_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)

SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

SET (ETERNITY_SOURCES
		Problem.h
		Problem.cpp
		initSolution.h
		initSolution.cpp
		evalSolution.h
		evalSolution.cpp
		Solution.h
		Solution.cpp
		Piece.h
)

ADD_LIBRARY(leternity STATIC ${ETERNITY_SOURCES})
