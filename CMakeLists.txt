cmake_minimum_required(VERSION 3.14)
project(ice_cxx)

cmake_policy(SET CMP0077 NEW)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)

include(cmake/library.cmake)
include(cmake/install.cmake)

set(ENV{HTTP_PROXY} "0.0.0.0:7890")
set(ENV{HTTPS_PROXY} "0.0.0.0:7890")

#add_libraries(cpptrace https://github.com/jeremy-rifkin/cpptrace.git v0.3.1)
add_libraries(string_view_lite https://github.com/martinmoene/string-view-lite.git v1.7.0)
add_libraries(spdlog https://github.com/gabime/spdlog.git v1.12.0)
add_libraries(googletest https://github.com/google/googletest.git v1.12.0)
add_libraries(span https://github.com/martinmoene/span-lite.git v0.10.3)
add_libraries_with_args(expected https://github.com/TartanLlama/expected.git v1.1.0 "BUILD_TESTING=OFF")
add_libraries_with_args(optional https://github.com/TartanLlama/optional.git v1.1.0 "BUILD_TESTING=OFF")
add_libraries_with_args(async https://e.coding.net/oaho/cpp/async.git v1.1.0 "ENABLE_EXAMPLE=OFF;ENABLE_TEST=OFF;ENABLE_SSL=OFF")

set(SRC_LISTS ${PROJECT_SOURCE_DIR}/src/stun)
foreach (dir ${SRC_LISTS})
    aux_source_directory(${dir} SRC_LIST)
endforeach()

add_library(ice_cxx ${SRC_LIST})
target_link_libraries(ice_cxx PUBLIC spdlog::spdlog PUBLIC async PUBLIC expected PUBLIC optional PUBLIC string-view-lite PUBLIC span-lite)
target_include_directories(ice_cxx PUBLIC ${PROJECT_SOURCE_DIR}/src)