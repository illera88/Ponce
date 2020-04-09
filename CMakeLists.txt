  
cmake_minimum_required(VERSION 3.15.0)
# MSVC runtime library flags are selected by an abstraction
# https://cmake.org/cmake/help/latest/policy/CMP0091.html
cmake_policy(SET CMP0091 NEW) 
cmake_policy(SET CMP0067 NEW)

option(STATICLIB "Build a static library. Recommended when building shipping ready plugin" OFF)
option(BUILD_EXAMPLES "Build examples" ON)
set(IDA_INSTALLED_DIR "" CACHE PATH "Path to directory where IDA is installed. If set, triton plugin will be moved there after building")

set (PROJECT_NAME Ponce)
project(${PROJECT_NAME})


file(GLOB PONCE_SOURCE_FILES
    src/*.cpp
)
file(GLOB PONCE_HEADER_FILES
    src/*.hpp
)

# ToDo add static versin
add_library(${PROJECT_NAME} SHARED ${PONCE_SOURCE_FILES} ${PONCE_HEADER_FILES})

#                       #
# Look for dependencies #
#                       # 

# Look for IDA SDK 
set(IDASDK_ROOT_DIR "" CACHE PATH "Path to directory idasdk7X where you extracted idasdk7X.zip")

set(IDA_INCLUDE_DIR ${IDASDK_ROOT_DIR}/include)
set(IDA_LIBRARIES ${IDASDK_ROOT_DIR}/lib/x64_win_vc_64/ida.lib ${IDASDK_ROOT_DIR}/lib/x64_win_vc_64/pro.lib)

# Find Triton
set(TRITON_INCLUDE_DIR "" CACHE PATH "Path to triton include directory")
set(TRITON_LIBRARY "" CACHE FILEPATH "Path to triton library")

# Look for Boost
find_package(Boost 1.55.0 REQUIRED)


if(BUILD_EXAMPLES)
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	add_executable(crackme_hash examples/crackme_hash.cpp)
	add_executable(crackme_xor examples/crackme_xor.cpp)
	add_executable(fread_SAGE examples/fread_SAGE.cpp)
	set_target_properties(crackme_hash
		PROPERTIES
		FOLDER "Examples")
	set_target_properties(crackme_xor
		PROPERTIES
		FOLDER "Examples")
	set_target_properties(fread_SAGE
		PROPERTIES
		FOLDER "Examples")
endif()

if(STATICLIB)
    # If we compile it statically we need to provide the all the libraries
	# Find Capstone
	find_package(Capstone REQUIRED)
	find_package(PkgConfig REQUIRED)
	#pkg_check_modules (CAPSTONE REQUIRED capstone)
	
	find_package(z3 CONFIG REQUIRED)
	
	set(CAPSTONE_LIBRARY "" CACHE FILEPATH "Path to capstone library")


	target_link_libraries(
		${PROJECT_NAME}
		PRIVATE
		${CAPSTONE_LIBRARY}
		z3::libz3
	)
endif()


# Now we create the project
target_include_directories(${PROJECT_NAME} PRIVATE ${TRITON_INCLUDE_DIR} ${IDA_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})

target_link_libraries(
    ${PROJECT_NAME}
	PRIVATE
    ${TRITON_LIBRARY}
	${IDA_LIBRARIES}
)


if(MSVC)
  add_definitions(/MP)
endif()

# Set preprocessor definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE __IDP__)
if(WIN32)
	target_compile_definitions(${PROJECT_NAME} PRIVATE __NT__)
	set(PLUGIN_EXTENSION dll)
    # for Windows operating system in general
elseif (APPLE)
	target_compile_definitions(${PROJECT_NAME} PRIVATE __MAC__)
	set(PLUGIN_EXTENSION dylib)
elseif (UNIX AND NOT APPLE)
	target_compile_definitions(${PROJECT_NAME} PRIVATE __LINUX__)
	set(PLUGIN_EXTENSION so)
endif()

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(PLUGIN_ARCH 64)
    target_compile_definitions(${PROJECT_NAME} PRIVATE __X64__)
else()
	target_compile_definitions(${PROJECT_NAME} PRIVATE __X86__)
endif()


if(IDA_INSTALLED_DIR)
	message(STATUS "[-] Ponce built plugin will be moved to '${IDA_INSTALLED_DIR}/plugins/'. The build system should have permisions to write there or it will error.")
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${IDA_INSTALLED_DIR}/plugins/${PROJECT_NAME}${PLUGIN_ARCH}.${PLUGIN_EXTENSION}
		COMMENT "Created ${IDA_INSTALLED_DIR}/plugins/${PROJECT_NAME}.${PLUGIN_EXTENSION}"
	)
endif()