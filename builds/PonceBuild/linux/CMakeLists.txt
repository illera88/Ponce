cmake_minimum_required(VERSION 2.8)
project(ponce)

cmake_policy(SET CMP0015 NEW)

set(CMAKE_BUILD_TYPE Release)

#since IDA does not name libraries with the lib prefix
#SET_TARGET_PROPERTIES(ponce PROPERTIES PREFIX "")

if (WIN32)
# Use VS
 message( FATAL_ERROR "Use Visual Studio project to build in Windows" )

elseif (APPLE)

    add_definitions(-D__MAC__=1)

    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -Wno-deprecated-writable-strings")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32 -Wno-deprecated-writable-strings")

    find_library (IDA_LIB
                  NAMES "ida"
                  PATHS "../../../Ponce/lib/x86_mac_gcc_32")

elseif (UNIX)

    add_definitions(-D__LINUX__=1)

    #Because Bosst uses sprintf_s
    add_definitions(-DUSE_DANGEROUS_FUNCTIONS)

    add_definitions(-D__IDP__)

    #IDA produce this kind of warnings
    add_definitions(-Wno-attributes)

    set (CMAKE_CXX_FLAGS "-m32 -std=c++0x -pipe -O2")

    set (CMAKE_FIND_LIBRARY_SUFFIXES ".a;.so")

    link_directories("../../../external-libs/libs/linux/ALL_static_x86/")


    find_library(IDA_LIB
                 NAMES "pro"
                 PATHS "../../../Ponce/lib/x86_linux_gcc_32/" 
		 NO_DEFAULT_PATH)

endif ( )

# Define all source files
file(
    GLOB_RECURSE
    PONCE_SOURCE_FILES
    "../../../Ponce/src/*"
)

# Define all header files
file(
    GLOB_RECURSE
    PONCE_HEADER_FILES
    "../../../Ponce/src/*.hpp"
)

# Include IDA libs
include_directories("../../../Ponce/include/")

# Include Triton libs
include_directories("../../../Triton/src/libtriton/includes/")

# Include Boost
include_directories("../../../external-libs/includes/boost_1_60_0/")

# Include Z3
include_directories("../../../external-libs/includes/z3-4.4.2-linux86/include")

#SET_TARGET_PROPERTIES(ponce PROPERTIES PREFIX "")

message (STATUS "Include directory: " ${IDA_SDK}/include/)
message (STATUS "IDA_SDK: " ${IDA_SDK})
message (STATUS "IDA_LIB: " ${IDA_LIB})

add_library(ponce MODULE ${PONCE_SOURCE_FILES})
#set_target_properties(ponce PROPERTIES COMPILE_FLAGS ${LIBTRITON_CXX_FLAGS})

if (WIN32)
    set (IDA_SUFFIX ".plw")
elseif (APPLE)
    set (IDA_SUFFIX ".pmc")
elseif (UNIX)
    set (IDA_SUFFIX ".plx")
endif()

# include ida library
target_link_libraries (ponce ${IDA_LIB})

#include z3 capstone and triton
target_link_libraries (ponce triton z3 capstone)
