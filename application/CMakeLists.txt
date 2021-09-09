######################################################################################
### 1) Include the sources
######################################################################################

include_directories(${PARADISEO_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)

######################################################################################
### 3) Define your target: just an executable here
######################################################################################

#EXECUTE_PROCESS(
#	   	COMMAND ${CMAKE_COMMAND} -E copy_if_different
#			${CMAKE_SOURCE_DIR}/application/VRP_HC.param
#			${CMAKE_BINARY_DIR}/application/VRP_HC.param
#)

ADD_EXECUTABLE(hopla main.cpp)
ADD_DEPENDENCIES(hopla leternity)

#ADD_EXECUTABLE(AG AG.cpp)
#ADD_DEPENDENCIES(AG leternity)

######################################################################################


######################################################################################
### 4) Link the librairies for your executable
######################################################################################

# Only if you need to link libraries
TARGET_LINK_LIBRARIES(hopla /Users/maximilienmoraud/Documents/IMTLD/5a/PROJET/ParadisEO-2.0/build/eo/lib/libeo.a leternity /Users/maximilienmoraud/Documents/IMTLD/5a/PROJET/ParadisEO-2.0/build/eo/lib/libeoutils.a)

# TARGET_LINK_LIBRARIES(AG eoutils eo leternity)

######################################################################################


######################################################################################
### 5) Copy the instances and the "param" file in the build path for an easy use.
### 
### --> run the "make install" target to copy the parameter file / instances
###	in the directory where you build the application
######################################################################################

#ADD_CUSTOM_TARGET(install DEPENDS ${CMAKE_SOURCE_DIR}/application/NSGAII.param
	#			  ${CMAKE_SOURCE_DIR}/application)
#ADD_CUSTOM_COMMAND(
 #   TARGET install
  #  POST_BUILD
   # COMMAND ${CMAKE_COMMAND}
   # ARGS -E copy_if_different
    #  ${CMAKE_SOURCE_DIR}/application/NSGAII.param
     # ${CMAKE_BINARY_DIR}/application)

######################################################################################
