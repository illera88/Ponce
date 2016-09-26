cmake_minimum_required(VERSION 2.8)
project(ponce)

cmake_policy(SET CMP0015 NEW)

set(CMAKE_BUILD_TYPE Release)

if (NOT ARCH)
    set(ARCH "x86")
endif()
if (NOT Z3DYN)
    set(Z3DYN "No")
endif()

#Common things
add_definitions(-D__IDP__)
if(${ARCH} MATCHES "x64")
        add_definitions(-D__X_64__=)
endif()
#Because Boost uses sprintf_s
add_definitions(-DUSE_DANGEROUS_FUNCTIONS)


if (WIN32)
# Use VS
 message( FATAL_ERROR "Use Visual Studio project to build in Windows" )

elseif (APPLE)

    add_definitions(-D__MAC__=1)

    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -m32 -Wno-deprecated-writable-strings")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32 -Wno-deprecated-writable-strings")

    if(${Z3DYN} MATCHES "Yes")
       link_directories("../../../external-libs/libs/osx/Z3_dyn_rest_static_x86/")
    else()
       link_directories("../../../external-libs/libs/osx/ALL_static_x86/")
    endif()

elseif (UNIX)

    add_definitions(-D__LINUX__=1)

    #IDA produce this kind of warnings
    add_definitions(-Wno-attributes)

    set (CMAKE_CXX_FLAGS "-m32 -std=c++0x -pipe -O2")

    set (CMAKE_FIND_LIBRARY_SUFFIXES ".a;.so")

    if(${Z3DYN} MATCHES "Yes")
       link_directories("../../../external-libs/libs/linux/Z3_dyn_rest_static_x86/")
    else()
       link_directories("../../../external-libs/libs/linux/ALL_static_x86/")
    endif()

endif ( )

# Define all source files
file(
    GLOB_RECURSE
    PONCE_SOURCE_FILES
    "../../../Ponce/src/*"
)

# Include IDA libs
include_directories("../../../Ponce/include/")

# Include Triton libs
include_directories("../../../Triton/src/libtriton/includes/")

# Include Boost
include_directories("../../../external-libs/includes/boost_1_60_0/")

# Include Z3
include_directories("../../../external-libs/includes/z3-4.4.2-linux86/include")

add_library(ponce MODULE ${PONCE_SOURCE_FILES})

if (APPLE)
    if(${ARCH} MATCHES "x64")
        set (IDA_SUFFIX ".pmc64")
    else()
	    set (IDA_SUFFIX ".pmc")
	endif()
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
SET_TARGET_PROPERTIES(ponce PROPERTIES PREFIX "")

# include ida library
target_link_libraries (ponce ${IDA_LIB})

#include z3 capstone and triton
target_link_libraries (ponce triton z3 capstone)

# Necesary include if compiling z3 with OMP
target_link_libraries(ponce gomp)

#Little trick to include IDA library since  it doesn't start with lib
if (APPLE)
    if(${ARCH} MATCHES "x64")
        target_link_libraries(ponce "${CMAKE_CURRENT_LIST_DIR}/../../../Ponce/lib/x64_mac_gcc_64/pro.a")
    else()
	    target_link_libraries(ponce "${CMAKE_CURRENT_LIST_DIR}/../../../Ponce/lib/x86_mac_gcc_32/pro.a")
	endif()
elseif (UNIX)
    if(${ARCH} MATCHES "x64")
        target_link_libraries(ponce "${CMAKE_CURRENT_LIST_DIR}/../../../Ponce/lib/x64_linux_gcc_64/pro.a")
    else()
	    target_link_libraries(ponce "${CMAKE_CURRENT_LIST_DIR}/../../../Ponce/lib/x86_linux_gcc_32/pro.a")
	endif()
endif()


