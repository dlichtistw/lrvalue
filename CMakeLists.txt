cmake_minimum_required(VERSION 3.14)
project(lrvalue VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

add_library(test_util test_util.cpp)

add_executable(construction_test construction_test.cpp)
target_link_libraries(construction_test test_util)
add_test(NAME construction_test COMMAND construction_test)

add_executable(conversion_test conversion_test.cpp)
target_link_libraries(conversion_test test_util)
add_test(NAME conversion_test COMMAND conversion_test)

add_executable(interface_test interface_test.cpp)
target_link_libraries(interface_test test_util)
add_test(NAME interface_test COMMAND function_call_test)

add_executable(boccara_test boccara_test.cpp)
add_test(NAME boccara_test COMMAND boccara_test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
