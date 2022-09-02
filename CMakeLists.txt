cmake_minimum_required(VERSION 3.0.0)
project(lrvalue VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(test_util test_util.cpp)

add_executable(construction_test construction_test.cpp)
target_link_libraries(construction_test test_util)
add_test(NAME construction_test COMMAND construction_test)

add_executable(boccara_test boccara_test.cpp)
add_test(NAME boccara_test COMMAND boccara_test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
