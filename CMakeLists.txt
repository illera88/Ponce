  
cmake_minimum_required(VERSION 3.15.0)
# MSVC runtime library flags are selected by an abstraction
# https://cmake.org/cmake/help/latest/policy/CMP0091.html
cmake_policy(SET CMP0091 NEW) 
cmake_policy(SET CMP0067 NEW)

option(STATICLIB "Build a static library. Recommended when building shipping ready plugin" OFF)

set (PROJECT_NAME Ponce)
project(${PROJECT_NAME})


file(GLOB PONCE_SOURCE_FILES
    Ponce/src/*.cpp
)
file(GLOB PONCE_HEADER_FILES
    Ponce/src/*.hpp
)

# ToDo add static versin
add_library(${PROJECT_NAME} SHARED ${PONCE_SOURCE_FILES} ${PONCE_HEADER_FILES})

#                       #
# Look for dependencies #
#                       # 

# Look for IDA SDK 
set(IDA_ROOT_DIR "" CACHE PATH "Path to directory idasdk7X where you extracted idasdk7X.zip")

set(IDA_INCLUDE_DIR PATH ${IDA_ROOT_DIR}/include)
set(IDA_LIBRARIES PATH ${IDA_ROOT_DIR}/lib/x64_win_vc_64/ida.lib ${IDA_ROOT_DIR}/lib/x64_win_vc_64/pro.lib)

message( ${IDA_INCLUDE_DIR})
# Find Triton
set(TRITON_INCLUDE_DIR "" CACHE PATH "Path to triton include directory")
set(TRITON_LIBRARY "" CACHE FILEPATH "Path to triton library")

# Look for Boost
#find_package(Boost 1.55.0 COMPONENTS multiprecision numeric-conversion REQUIRED)

if(NOT (TRITON_INCLUDE_DIR AND TRITON_LIBRARY))
		message(FATAL_ERROR "Missing TRITON_INCLUDE_DIR and/or TRITON_LIBRARY")
	endif()

if(STATICLIB)
    # If we compile it statically we need to provide the all the libraries
	# Find Capstone
	set(CAPSTONE_LIBRARY "" CACHE FILEPATH "Path to capstone library")

	if(NOT (IDA_ROOT_DIR))
		message(FATAL_ERROR "Path required for IDA_ROOT_DIR (idasdk7X where you extracted idasdk7X.zip)")
	endif()

	if(NOT (CAPSTONE_LIBRARY))
		message(FATAL_ERROR "Missing CAPSTONE_LIBRARY")
	endif()

	# Find Boost
	set(BOOST_ROOT "" CACHE PATH "Path to Boost root directory")

	if(NOT (BOOST_ROOT))
		message(FATAL_ERROR "Missing BOOST_ROOT")
	endif()

endif()
# Now we create the project

target_include_directories(${PROJECT_NAME} PRIVATE ${TRITON_INCLUDE_DIR} ${IDA_INCLUDE_DIR} )

target_link_libraries(
    ${PROJECT_NAME}
    ${CAPSTONE_LIBRARY}
    ${TRITON_LIBRARY}
	${IDA_LIBRARIES}
)

# Set preprocessor definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE __IDP__)
if(WIN32)
	target_compile_definitions(${PROJECT_NAME} PRIVATE __NT__)
    # for Windows operating system in general
elseif (APPLE)
	target_compile_definitions(${PROJECT_NAME} PRIVATE __MAC__)
elseif (UNIX AND NOT APPLE)
	target_compile_definitions(${PROJECT_NAME} PRIVATE __LINUX__)
endif()

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_compile_definitions(${PROJECT_NAME} PRIVATE __X64__)
else()
	target_compile_definitions(${PROJECT_NAME} PRIVATE __X86__)
endif()

if(STATICLIB_RR)
	
	set(IDA_ROOT_DIR "" CACHE PATH "Path to directory idasdk7X where you extracted idasdk7X.zip")

	if(NOT (IDA_ROOT_DIR))
		message(FATAL_ERROR "Path required for IDA_ROOT_DIR (idasdk7X where you extracted idasdk7X.zip)")
	endif()

	# Look for z3

	# Look for Capstone
	include(FindPkgConfig)
	pkg_check_modules (CAPSTONE REQUIRED capstone)
	# Use CAPSTONE_FOUND, CAPSTONE_LIBRARIES, CAPSTONE_INCLUDE_DIRS vars
	target_link_libraries(${PROJECT_NAME} ${CAPSTONE_LIBRARIES})
endif()