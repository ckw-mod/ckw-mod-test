cmake_minimum_required(VERSION 2.8)
project(ckw-mod-test CXX)

enable_testing()

include_directories(
    ${CMAKE_SOURCE_DIR}/ckw
    ${CMAKE_SOURCE_DIR}/gtest/include
    ${CMAKE_SOURCE_DIR}/gtest)

set(SRCS 
    option_test.cpp 
    ${CMAKE_SOURCE_DIR}/gtest/src/gtest-all.cc
    ${CMAKE_SOURCE_DIR}/gtest/src/gtest_main.cc)
set(LIBS shlwapi)

add_executable(option_test ${SRCS})
target_link_libraries(option_test ${LIBS})

add_test(NAME option_test
         WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
         COMMAND option_test)
