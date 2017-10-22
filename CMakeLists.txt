cmake_minimum_required(VERSION 3.1)
project(functional-cpp)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/catch/single_include)
set(JSON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/json/src)
set(FP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(FP_INCLUDE_DIR ${FP_INCLUDE_DIR} PARENT_SCOPE)

macro(fp_option OPTION_NAME OPTION_TEXT OPTION_DEFAULT)
    option(${OPTION_NAME} ${OPTION_TEXT} ${OPTION_DEFAULT})
    if(DEFINED ENV{${OPTION_NAME}})
        # Allow setting the option through an environment variable
        set(${OPTION_NAME} $ENV{${OPTION_NAME}})
    endif()
    if(${OPTION_NAME})
        add_definitions(-D${OPTION_NAME})
    endif()
    message(STATUS "  ${OPTION_NAME}: ${${OPTION_NAME}}")
endmacro()

fp_option(BUILD_FP_EXEC "Set to ON to build fp exec." ON)
fp_option(BUILD_FP_TESTS "Set to ON to build fp tests." ON)

include_directories(
  ${CATCH_INCLUDE_DIR}
  ${JSON_INCLUDE_DIR}
  ${FP_INCLUDE_DIR}
  )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")

if(BUILD_FP_EXEC)
  add_executable(fp_exec example/example.cc)
endif()

if(BUILD_FP_TESTS)
  add_executable(fp_tests test/tests.cc)
  enable_testing()
  add_test(NAME Tests COMMAND fp_tests)
endif()

if(BUILD_FP_EXEC AND BUILD_FP_TESTS)
  add_dependencies(fp_exec fp_tests)
endif()
