  
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

if(NOT IDASDK_ROOT_DIR)
	message(FATAL_ERROR "You should set IDASDK_ROOT_DIR to the IDA SDK path")
endif()

set(IDA_INCLUDE_DIR ${IDASDK_ROOT_DIR}/include)

# IDA SDK libs.
if(WIN32)
	set(idasdk_ea32 "${IDASDK_ROOT_DIR}/lib/x64_win_vc_32/ida.lib")
	set(idasdk_ea64 "${IDASDK_ROOT_DIR}/lib/x64_win_vc_64/ida.lib")
elseif(APPLE)
	set(idasdk_ea32 "${IDASDK_ROOT_DIR}/lib/x64_mac_gcc_32/libida.dylib")
	set(idasdk_ea64 "${IDASDK_ROOT_DIR}/lib/x64_mac_gcc_64/libida64.dylib")
elseif(UNIX) # APPLE is also UNIX, so it MUST be before this elseif().
	set(idasdk_ea32 "${IDASDK_ROOT_DIR}/lib/x64_linux_gcc_32/libida.so")
	set(idasdk_ea64 "${IDASDK_ROOT_DIR}/lib/x64_linux_gcc_64/libida64.so")
else()
	message(FATAL_ERROR "Unsupported system type: ${CMAKE_SYSTEM_NAME}")
endif()

# Find Triton
set(TRITON_INCLUDE_DIR "" CACHE PATH "Path to triton include directory")
set(TRITON_LIBRARY "" CACHE FILEPATH "Path to triton library")

if(NOT TRITON_INCLUDE_DIR OR NOT TRITON_LIBRARY)
	message(FATAL_ERROR "You should set Triton libraries")
endif()

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
	find_library(CAPSTONE_LIBRARY capstone REQUIRED)
	find_path(Capstone_INCLUDE_DIR capstone.h PATH_SUFFIXES capstone)

	if(NOT MSVC)
		set(Z3_LIBRARY "" CACHE FILEPATH "Path to Z3 library")
		if(NOT Z3_LIBRARY)
			message(FATAL_ERROR "You should set z3 libraries")
		endif()
		
		target_link_libraries(
			${PROJECT_NAME}
			PRIVATE
			${Z3_LIBRARY}
			${CAPSTONE_LIBRARY}
		)

	else()
		  # If using the static library we should use the static runtime too
		set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")

		# Using Capstone from vcpkg has this caveat
		set(${Capstone_INCLUDE_DIR} ${Capstone_INCLUDE_DIR}../)
		
		# Find z3
		find_package(z3 CONFIG REQUIRED)

		target_link_libraries(
			${PROJECT_NAME}
			PRIVATE
			z3::libz3
			${CAPSTONE_LIBRARY}
		)
	endif()
	
	
	target_include_directories(${PROJECT_NAME} PRIVATE ${Capstone_INCLUDE_DIR})
endif()


# Now we create the project
target_include_directories(${PROJECT_NAME} PRIVATE ${TRITON_INCLUDE_DIR} ${IDA_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})

target_link_libraries(
    ${PROJECT_NAME}
	PRIVATE
    ${TRITON_LIBRARY}
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
	target_compile_definitions(${PROJECT_NAME} PRIVATE USE_DANGEROUS_FUNCTIONS)
	set(PLUGIN_EXTENSION so)
endif()

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(PLUGIN_ARCH 64)
    target_compile_definitions(${PROJECT_NAME} PRIVATE __X64__ __EA64__)
	target_link_libraries(
    ${PROJECT_NAME}
	PRIVATE
    ${idasdk_ea64}
)

else()
	target_compile_definitions(${PROJECT_NAME} PRIVATE __X86__)
	target_link_libraries(
    ${PROJECT_NAME}
	PRIVATE
    ${idasdk_ea32}
)

endif()


if(IDA_INSTALLED_DIR)
	message(STATUS "[-] Ponce built plugin and pdb file will be moved to '${IDA_INSTALLED_DIR}/plugins/'. The build system should have permisions to write there or it will error.")
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${IDA_INSTALLED_DIR}/plugins/${PROJECT_NAME}${PLUGIN_ARCH}.${PLUGIN_EXTENSION}
		COMMENT "Created ${IDA_INSTALLED_DIR}/plugins/${PROJECT_NAME}.${PLUGIN_EXTENSION}"
	)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/Debug/${PROJECT_NAME}.pdb ${IDA_INSTALLED_DIR}/plugins/${PROJECT_NAME}.pdb
		COMMENT "Created ${IDA_INSTALLED_DIR}/plugins/${PROJECT_NAME}.${PLUGIN_EXTENSION}"
	)
endif()
