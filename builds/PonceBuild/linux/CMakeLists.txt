cmake_minimum_required(VERSION 2.8)
project(ponce)

cmake_policy(SET CMP0015 NEW)

set(CMAKE_BUILD_TYPE Release)

#option(ARCH "Build arquitecture" "x86") # Use -DARCH=x64 for 64 bits build
#option(Z3DYN "Use Z3 dynamyc library" OFF) # Use -DZ3DYN=ON to use the z3 dynamic lib
if (NOT ARCH)
    set(ARCH "x86")
endif()
if (NOT Z3DYN)
    set(Z3DYN "No")
endif()



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
    add_definitions(-D__IDP__)

    if(${ARCH} MATCHES "x64")
        add_definitions(-D__X_64__=)
    endif()

    #Because Boost uses sprintf_s
    add_definitions(-DUSE_DANGEROUS_FUNCTIONS)

    #IDA produce this kind of warnings
    add_definitions(-Wno-attributes)

    set (CMAKE_CXX_FLAGS "-m32 -std=c++0x -pipe -O2")

    set (CMAKE_FIND_LIBRARY_SUFFIXES ".a;.so")

    if(${Z3DYN} MATCHES "Yes")
       link_directories("../../../external-libs/libs/linux/Z3_dyn_rest_static_x86/")
    else()
       link_directories("../../../external-libs/libs/linux/ALL_static_x86/")
    endif()

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

add_library(ponce MODULE ${PONCE_SOURCE_FILES})
#set_target_properties(ponce PROPERTIES COMPILE_FLAGS ${LIBTRITON_CXX_FLAGS})

if (WIN32)
    set (IDA_SUFFIX ".plw")
elseif (APPLE)
    set (IDA_SUFFIX ".pmc")
elseif (UNIX)
    if(${ARCH} MATCHES "x64")
        set (IDA_SUFFIX ".plx64")
    else()
        set (IDA_SUFFIX ".plx")
    endif()
endif()

#Print some info
message (STATUS "Include directory: " ${IDA_SDK}/include/)
message (STATUS "IDA_LIB: " ${IDA_LIB})
message (STATUS "IDA_SUFFIX: " ${IDA_SUFFIX})
message (STATUS "ARCH: " ${ARCH})
message (STATUS "Dynamic Z3: " ${Z3DYN})


set_target_properties (ponce PROPERTIES SUFFIX ${IDA_SUFFIX})

# include ida library
target_link_libraries (ponce ${IDA_LIB})

#include z3 capstone and triton
target_link_libraries (ponce triton z3 capstone)

# Necesary include if compiling z3 with OMP
target_link_libraries(ponce gomp)
