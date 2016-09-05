# Install script for directory: /home/default/TritonProject/Triton

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/lib/libtriton.a")
FILE(INSTALL DESTINATION "/lib" TYPE STATIC_LIBRARY FILES "/home/default/TritonProject/Triton/build/linux/x64/build/libtriton.a")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/usr/include/triton/memoryOperand.hpp;/usr/include/triton/astPythonRepresentation.hpp;/usr/include/triton/pathManager.hpp;/usr/include/triton/astDictionaries.hpp;/usr/include/triton/x86Cpu.hpp;/usr/include/triton/solverModel.hpp;/usr/include/triton/pythonBindings.hpp;/usr/include/triton/astVisitor.hpp;/usr/include/triton/operandWrapper.hpp;/usr/include/triton/symbolicVariable.hpp;/usr/include/triton/astEnums.hpp;/usr/include/triton/instruction.hpp;/usr/include/triton/bitsVector.hpp;/usr/include/triton/version.hpp.in;/usr/include/triton/externalLibs.hpp;/usr/include/triton/x8664Cpu.hpp;/usr/include/triton/tritonToZ3Ast.hpp;/usr/include/triton/coreUtils.hpp;/usr/include/triton/pythonXFunctions.hpp;/usr/include/triton/operandInterface.hpp;/usr/include/triton/api.hpp;/usr/include/triton/astRepresentationInterface.hpp;/usr/include/triton/solverEngine.hpp;/usr/include/triton/tritonTypes.hpp;/usr/include/triton/cpuSize.hpp;/usr/include/triton/z3ToTritonAst.hpp;/usr/include/triton/astSmtRepresentation.hpp;/usr/include/triton/symbolicEnums.hpp;/usr/include/triton/astRepresentation.hpp;/usr/include/triton/pythonObjects.hpp;/usr/include/triton/symbolicExpression.hpp;/usr/include/triton/symbolicEngine.hpp;/usr/include/triton/symbolicOptimization.hpp;/usr/include/triton/pythonUtils.hpp;/usr/include/triton/symbolicSimplification.hpp;/usr/include/triton/ast.hpp;/usr/include/triton/x86Semantics.hpp;/usr/include/triton/syscalls.hpp;/usr/include/triton/taintEngine.hpp;/usr/include/triton/x86Specifications.hpp;/usr/include/triton/version.hpp;/usr/include/triton/registerOperand.hpp;/usr/include/triton/immediateOperand.hpp;/usr/include/triton/pathConstraint.hpp;/usr/include/triton/cpuInterface.hpp;/usr/include/triton/architecture.hpp;/usr/include/triton/z3Result.hpp;/usr/include/triton/unix.hpp")
FILE(INSTALL DESTINATION "/usr/include/triton" TYPE FILE FILES
    "/home/default/TritonProject/Triton/src/libtriton/includes/memoryOperand.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/astPythonRepresentation.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/pathManager.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/astDictionaries.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/x86Cpu.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/solverModel.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/pythonBindings.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/astVisitor.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/operandWrapper.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/symbolicVariable.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/astEnums.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/instruction.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/bitsVector.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/version.hpp.in"
    "/home/default/TritonProject/Triton/src/libtriton/includes/externalLibs.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/x8664Cpu.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/tritonToZ3Ast.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/coreUtils.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/pythonXFunctions.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/operandInterface.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/api.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/astRepresentationInterface.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/solverEngine.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/tritonTypes.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/cpuSize.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/z3ToTritonAst.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/astSmtRepresentation.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/symbolicEnums.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/astRepresentation.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/pythonObjects.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/symbolicExpression.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/symbolicEngine.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/symbolicOptimization.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/pythonUtils.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/symbolicSimplification.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/ast.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/x86Semantics.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/syscalls.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/taintEngine.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/x86Specifications.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/version.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/registerOperand.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/immediateOperand.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/pathConstraint.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/cpuInterface.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/architecture.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/z3Result.hpp"
    "/home/default/TritonProject/Triton/src/libtriton/includes/unix.hpp"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/home/default/TritonProject/Triton/build/linux/x64/build/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/home/default/TritonProject/Triton/build/linux/x64/build/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
