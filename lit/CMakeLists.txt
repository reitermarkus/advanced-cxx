cmake_minimum_required(VERSION 3.17)

project(lit VERSION 1.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(BuildType)
include(CxxStandard)

add_executable(lit)
target_sources(lit
  PRIVATE
    src/main.cxx
    src/command.cxx
    src/fs.cxx
    src/patch.cxx
    src/dir_diff.cxx
    src/revision.cxx
)
target_include_directories(lit PUBLIC "${CMAKE_CURRENT_LIST_DIR}/inc")
