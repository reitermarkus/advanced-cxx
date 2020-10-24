cmake_minimum_required(VERSION 3.17)

project(lit VERSION 1.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(BuildType)
include(CxxStandard)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
include(ClangTidy)
endif()

add_executable(lit)
target_sources(lit PRIVATE src/main.cxx)
target_include_directories(lit PUBLIC "${CMAKE_CURRENT_LIST_DIR}/inc")
