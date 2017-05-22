cmake_minimum_required(VERSION 3.0)
project(avakar_metapp)

include(deps.cmake)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(avakar_metapp INTERFACE)
target_include_directories(
    avakar_metapp
    INTERFACE include
    )

add_executable(avakar_metapp_test EXCLUDE_FROM_ALL
    test/test_meta.cpp
    )
target_link_libraries(avakar_metapp_test avakar_metapp mutest)

source_group(include REGULAR_EXPRESSION include/.*)
source_group(src REGULAR_EXPRESSION src/.*)
source_group(test REGULAR_EXPRESSION test/.*)
