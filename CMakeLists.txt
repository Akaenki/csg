cmake_minimum_required(VERSION 3.1)

project(votca-csg)

set(PROJECT_VERSION "1.5-dev")
set(PROJECT_CONTACT "bugs@votca.org")
string(REGEX REPLACE "^[1-9]+\\.([1-9]+).*$" "\\1" SOVERSION "${PROJECT_VERSION}")
if (NOT ${SOVERSION} MATCHES "[1-9]+")
  message(FATAL_ERROR "Could not determind SOVERSION from ${PROJECT_VERSION}")
endif (NOT ${SOVERSION} MATCHES "[1-9]+")

# Cmake modules/macros are in a subdirectory to keep this file cleaner
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CXX_FLAGS)
  #Release comes with -O3 by default
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CXX_FLAGS)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  add_definitions(-DDEBUG)
endif(CMAKE_BUILD_TYPE STREQUAL Debug)

enable_language(CXX)

######################################################################
# compiler tests
# these need ot be done early (before further tests).
#####################################################################

include(CheckCXXCompilerFlag)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel" AND CMAKE_VERSION VERSION_LESS "3.6.0")
  message(FATAL_ERROR "Using the intel compiler requires cmake 3.6 or higher")
endif()

########################################################################
# User input options                                                   #
########################################################################
option(BUILD_SHARED_LIBS "Build shared libs" ON)
include(GNUInstallDirs)

option(ENABLE_TESTING "Build and enable testing stuff" OFF)
if(ENABLE_TESTING)
  enable_testing()
endif(ENABLE_TESTING)

# this has to be the include specification!
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

########################################################################
#Find external packages
########################################################################
if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/.git)
  find_package(Git)
endif(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/.git)

find_package(Threads REQUIRED)
set(THREAD_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
option(BUILD_MANPAGES "Build manpages" ON)
if (BUILD_MANPAGES)
  find_package(TXT2TAGS)
  find_package(UnixCommands)
else (BUILD_MANPAGES)
  #we use TXT2TAGS_FOUND conditionals in other CMakeLists.txt
  #so that TXT2TAGS is never required
  set(TXT2TAGS_FOUND)
endif (BUILD_MANPAGES)

find_package(Boost 1.39.0 REQUIRED COMPONENTS program_options filesystem system )
include_directories(${Boost_INCLUDE_DIRS})
set (BOOST_CFLAGS_PKG "-I${Boost_INCLUDE_DIRS}")
set(BOOST_LIBS_PKG "-L${Boost_LIBRARY_DIRS}")
foreach(_blib ${Boost_LIBRARIES})
  string(REGEX REPLACE ".*/lib([^/]*)\\.[^.]*$" "-l\\1" _blib ${_blib})
  set(BOOST_LIBS_PKG "${BOOST_LIBS_PKG} ${_blib}")
endforeach(_blib)

find_package(VOTCA_TOOLS REQUIRED)
include_directories(${VOTCA_TOOLS_INCLUDE_DIRS})

option(WITH_GMX "Build gromacs reader/writer, disabling leads to reduced functionality!" ON)
if (WITH_GMX)
  find_package(GROMACS 20160000 REQUIRED)
endif(WITH_GMX) 

########################################################################
# Basic system tests (standard libraries, headers, functions, types)   #
########################################################################
include(CheckIncludeFile)
foreach(HEADER assert.h)
  check_include_file(${HEADER} FOUND_${HEADER})
  if(NOT FOUND_${HEADER})
    message(FATAL_ERROR "Could not find needed header - ${HEADER}")
  endif(NOT FOUND_${HEADER})
endforeach(HEADER)

include(CheckIncludeFileCXX)
foreach(HEADER algorithm fstream iomanip iostream list map numeric sstream stdexcept string vector cstdlib)
  check_include_file_cxx(${HEADER} FOUND_${HEADER})
  if(NOT FOUND_${HEADER})
    message(FATAL_ERROR "Could not find needed header - ${HEADER}")
  endif(NOT FOUND_${HEADER})
endforeach(HEADER)

set(MATH_LIBRARIES "m" CACHE STRING "math library")
mark_as_advanced( MATH_LIBRARIES )
include(CheckLibraryExists)
foreach(FUNC sqrt)
  check_library_exists(${MATH_LIBRARIES} ${FUNC} "" FOUND_${FUNC}_${MATH_LIBRARIES})
  if(NOT FOUND_${FUNC}_${MATH_LIBRARIES})
    message(FATAL_ERROR "Could not find needed math function - ${FUNC}")
  endif(NOT FOUND_${FUNC}_${MATH_LIBRARIES})
endforeach(FUNC)

######################################
# Include the following subdirectory # 
######################################
if(NOT TARGET manpages)
  add_custom_target(manpages ALL)
endif()
add_subdirectory(scripts)
add_subdirectory(src)
add_subdirectory(include/votca/csg)
add_subdirectory(share)

file(GLOB VOTCA_CMAKE_MODULES CMakeModules/FindVOTCA* CMakeModules/FindSQLITE3.cmake)
install(FILES ${VOTCA_CMAKE_MODULES} DESTINATION ${CMAKE_INSTALL_DATADIR}/votca/template/CMakeModules)

configure_file(${CMAKE_MODULE_PATH}/cmake_uninstall.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake IMMEDIATE @ONLY)
add_custom_target(uninstall-csg COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
if(NOT TARGET uninstall)
  add_custom_target(uninstall)
endif()
add_dependencies(uninstall uninstall-csg)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
  include(FeatureSummary)
  feature_summary(INCLUDE_QUIET_PACKAGES WHAT ALL)
endif (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
