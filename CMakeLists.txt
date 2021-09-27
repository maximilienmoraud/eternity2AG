######################################################################################
### 0) Set your application properties
######################################################################################
# check cmake version compatibility
cmake_minimum_required(VERSION 2.8)

# Here define your project name
PROJECT("eternity")

# Here define the name and the version of your package
SET(PACKAGE_NAME "eternity" CACHE STRING "Package name" FORCE)
SET(PACKAGE_VERSION "0.1" CACHE STRING "Package version" FORCE)


# regular expression checking
INCLUDE_REGULAR_EXPRESSION("^.*$" "^$")

# set a language for the entire project.
ENABLE_LANGUAGE(CXX)
ENABLE_LANGUAGE(C)

######################################################################################

######################################################################################
### 1) Include the install configuration file where are defined the main variables
######################################################################################
set(PARADISEO_ROOT ../ParadisEO-2.0)

include(CMakeBackwardCompatibilityCXX)

include(FindDoxygen)

include(CheckLibraryExists)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/module")
find_package(Paradiseo COMPONENTS moeo eoutils eo)

include_directories(${PARADISEO_INCLUDE_DIR})
              
######################################################################################

#####################################################################################
### Manage the build type
#####################################################################################

# the user should choose the build type on windows environments,excepted under cygwin (default=none)
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Variable that stores the default CMake build type" FORCE)

find_program(MEMORYCHECK_COMMAND
    NAMES purify valgrind
    PATHS
    "/usr/local/bin /usr/bin [HKEY_LOCAL_MACHINE\\SOFTWARE\\Rational Software\\Purify\\Setup;InstallFolder]"
    DOC "Path to the memory checking command, used for memory error detection.") 
 
      
if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DCMAKE_VERBOSE_MAKEFILE=ON)
endif(CMAKE_BUILD_TYPE MATCHES Debug)

add_definitions(-DDEPRECATED_MESSAGES)

######################################################################################

#####################################################################################
### Manage the build type
#####################################################################################

######################################################################################
### 3) Link the librairies for your executable
######################################################################################

ADD_SUBDIRECTORY(application)
ADD_SUBDIRECTORY(doc)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)


######################################################################################

