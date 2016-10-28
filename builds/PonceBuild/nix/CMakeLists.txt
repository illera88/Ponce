cmake_minimum_required(VERSION 2.8)
project(ponce)

cmake_policy(SET CMP0015 NEW)

set(CMAKE_BUILD_TYPE Release)

execute_process(COMMAND /bin/bash -c "cd ../../../../; /bin/bash ./version_number_update.sh")

if (NOT IDA_ARCH)
    set(IDA_ARCH "x86")
endif()
if (NOT IDA_VER)
    set(IDA_VER "69")
endif()
if (NOT IDA_INSTALLATION_PATH)
    set (IDA_INSTALLATION_PATH "/Applications/IDA Pro 6.95/")
endif()

#Common things
add_definitions(-D__IDP__)
if(${IDA_ARCH} MATCHES "x64")
    add_definitions(-D__EA64__=1)
endif()
#Because Boost uses sprintf_s
add_definitions(-DUSE_DANGEROUS_FUNCTIONS)

if (WIN32)
    # Use VS
    message( FATAL_ERROR "Use Visual Studio project to build in Windows" )
elseif (APPLE)
    add_definitions(-D__MAC__=1)

    #IDA produces this kind of warnings
    #add_definitions(-Wno-attributes)

    # cstdio uses them
    add_definitions(-DUSE_STANDARD_FILE_FUNCTIONS)

    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -static-libstdc++ -m32")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc -m32 ")
    
    link_directories("../../../external-libs/libs/osx/ALL_static_x86/")
elseif (UNIX)

    add_definitions(-D__LINUX__=1)

    #IDA produce this kind of warnings
    add_definitions(-Wno-attributes)

    set (CMAKE_CXX_FLAGS "-m32 -std=c++0x -static-libgcc -static-libstdc++ -pipe -O2")

    set (CMAKE_FIND_LIBRARY_SUFFIXES ".a;.so")
    
    link_directories("../../../external-libs/libs/linux/ALL_static_x86/")
endif ( )

#We use this flag to avoid functionality not working in IDA 6.8
if (${IDA_VER} MATCHES "68")
    add_definitions(-D__IDA68__=1)
endif()

# Define all source files
file(
    GLOB_RECURSE
    PONCE_SOURCE_FILES
    "../../../Ponce/src/*"
)

# Include IDA libs
if (${IDA_VER} MATCHES "68")
    include_directories("../../../Ponce/include_IDA6.8/")
else()
    include_directories("../../../Ponce/include_IDA6.95/")
endif()

# Include Triton libs
include_directories("../../../Triton/src/libtriton/includes/")

# Include Boost
include_directories("../../../external-libs/includes/boost_1_60_0/")

# Include Z3
include_directories("../../../external-libs/includes/z3-4.4.2-linux86/include")

add_library(ponce MODULE ${PONCE_SOURCE_FILES})

if (APPLE)
    if(${IDA_ARCH} MATCHES "x64")
        set (IDA_SUFFIX "_x64_IDA${IDA_VER}_mac.pmc64")
    else()
        set (IDA_SUFFIX "_x86_IDA${IDA_VER}_mac.pmc")
    endif()
elseif (UNIX)
    if(${IDA_ARCH} MATCHES "x64")
        set (IDA_SUFFIX "_x64_IDA${IDA_VER}_linux.plx64")
    else()
        set (IDA_SUFFIX "_x86_IDA${IDA_VER}_linux.plx")
    endif()
endif()

#Print some info
message (STATUS "IDA_LIB: " ${IDA_LIB})
message (STATUS "IDA_SUFFIX: " ${IDA_SUFFIX})
message (STATUS "IDA_ARCH: " ${IDA_ARCH})

set_target_properties (ponce PROPERTIES SUFFIX ${IDA_SUFFIX})
SET_TARGET_PROPERTIES(ponce PROPERTIES PREFIX "")

# include ida library
target_link_libraries (ponce ${IDA_LIB})

#include z3 capstone and triton
target_link_libraries (ponce triton z3 capstone)

# Necesary include if compiling z3 with OMP
#target_link_libraries(ponce gomp)

#The lib path
if (${IDA_VER} MATCHES "68")
    set(IDA_LIB_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../Ponce/lib_IDA6.8/")
else()
    set(IDA_LIB_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../Ponce/lib_IDA6.95/")
endif()

#Little trick to include IDA library since  it doesn't start with lib
if (APPLE)
    if(${IDA_ARCH} MATCHES "x64")
        target_link_libraries(ponce "${IDA_LIB_PATH}x86_mac_gcc_32/pro.a")
        target_link_libraries(ponce "${IDA_INSTALLATION_PATH}idaq.app/Contents/MacOS/libida64.dylib")
    else()
        target_link_libraries(ponce "${IDA_LIB_PATH}x86_mac_gcc_32/pro.a")
        target_link_libraries(ponce "${IDA_INSTALLATION_PATH}idaq.app/Contents/MacOS/libida.dylib")
    endif()
elseif (UNIX)
    if(${IDA_ARCH} MATCHES "x64")
        target_link_libraries(ponce "${IDA_LIB_PATH}x86_linux_gcc_32/pro.a")
    else()
        target_link_libraries(ponce "${IDA_LIB_PATH}x86_linux_gcc_32/pro.a")
    endif()
endif()
