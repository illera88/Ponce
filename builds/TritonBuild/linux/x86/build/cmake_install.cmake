# Install script for directory: /home/default/PonceProject/Triton

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/lib/libtriton.a")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/lib" TYPE STATIC_LIBRARY FILES "/home/default/PonceProject/builds/TritonBuild/linux/x86/build/libtriton.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/include/triton/binaryInterface.hpp;/usr/include/triton/api.hpp;/usr/include/triton/elfEnums.hpp;/usr/include/triton/pathConstraint.hpp;/usr/include/triton/version.hpp.in;/usr/include/triton/elfSymbolTable.hpp;/usr/include/triton/x8664Cpu.hpp;/usr/include/triton/operandWrapper.hpp;/usr/include/triton/astVisitor.hpp;/usr/include/triton/operandInterface.hpp;/usr/include/triton/symbolicEngine.hpp;/usr/include/triton/astGarbageCollector.hpp;/usr/include/triton/memoryMapping.hpp;/usr/include/triton/x86Specifications.hpp;/usr/include/triton/version.hpp;/usr/include/triton/cpuInterface.hpp;/usr/include/triton/astDictionaries.hpp;/usr/include/triton/symbolicSimplification.hpp;/usr/include/triton/tritonTypes.hpp;/usr/include/triton/pythonUtils.hpp;/usr/include/triton/taintEngine.hpp;/usr/include/triton/astSmtRepresentation.hpp;/usr/include/triton/architecture.hpp;/usr/include/triton/pythonXFunctions.hpp;/usr/include/triton/pathManager.hpp;/usr/include/triton/bitsVector.hpp;/usr/include/triton/astPythonRepresentation.hpp;/usr/include/triton/elf.hpp;/usr/include/triton/cpuSize.hpp;/usr/include/triton/z3Result.hpp;/usr/include/triton/syscalls.hpp;/usr/include/triton/register.hpp;/usr/include/triton/symbolicEnums.hpp;/usr/include/triton/astRepresentation.hpp;/usr/include/triton/exceptions.hpp;/usr/include/triton/elfRelocationTable.hpp;/usr/include/triton/unix.hpp;/usr/include/triton/callbacks.hpp;/usr/include/triton/pythonBindings.hpp;/usr/include/triton/pythonObjects.hpp;/usr/include/triton/ast.hpp;/usr/include/triton/immediate.hpp;/usr/include/triton/solverEngine.hpp;/usr/include/triton/x86Cpu.hpp;/usr/include/triton/elfSectionHeader.hpp;/usr/include/triton/tritonToZ3Ast.hpp;/usr/include/triton/z3ToTritonAst.hpp;/usr/include/triton/elfDynamicTable.hpp;/usr/include/triton/symbolicExpression.hpp;/usr/include/triton/abstractBinary.hpp;/usr/include/triton/coreUtils.hpp;/usr/include/triton/x86Semantics.hpp;/usr/include/triton/symbolicOptimization.hpp;/usr/include/triton/externalLibs.hpp;/usr/include/triton/astEnums.hpp;/usr/include/triton/astRepresentationInterface.hpp;/usr/include/triton/solverModel.hpp;/usr/include/triton/elfProgramHeader.hpp;/usr/include/triton/instruction.hpp;/usr/include/triton/memoryAccess.hpp;/usr/include/triton/symbolicVariable.hpp;/usr/include/triton/elfHeader.hpp")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/include/triton" TYPE FILE FILES
    "/home/default/PonceProject/Triton/src/libtriton/includes/binaryInterface.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/api.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/elfEnums.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/pathConstraint.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/version.hpp.in"
    "/home/default/PonceProject/Triton/src/libtriton/includes/elfSymbolTable.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/x8664Cpu.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/operandWrapper.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/astVisitor.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/operandInterface.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/symbolicEngine.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/astGarbageCollector.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/memoryMapping.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/x86Specifications.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/version.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/cpuInterface.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/astDictionaries.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/symbolicSimplification.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/tritonTypes.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/pythonUtils.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/taintEngine.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/astSmtRepresentation.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/architecture.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/pythonXFunctions.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/pathManager.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/bitsVector.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/astPythonRepresentation.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/elf.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/cpuSize.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/z3Result.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/syscalls.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/register.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/symbolicEnums.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/astRepresentation.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/exceptions.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/elfRelocationTable.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/unix.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/callbacks.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/pythonBindings.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/pythonObjects.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/ast.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/immediate.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/solverEngine.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/x86Cpu.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/elfSectionHeader.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/tritonToZ3Ast.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/z3ToTritonAst.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/elfDynamicTable.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/symbolicExpression.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/abstractBinary.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/coreUtils.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/x86Semantics.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/symbolicOptimization.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/externalLibs.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/astEnums.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/astRepresentationInterface.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/solverModel.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/elfProgramHeader.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/instruction.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/memoryAccess.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/symbolicVariable.hpp"
    "/home/default/PonceProject/Triton/src/libtriton/includes/elfHeader.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/default/PonceProject/builds/TritonBuild/linux/x86/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
