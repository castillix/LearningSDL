cmake_minimum_required(VERSION 3.16)
project(Transit)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Output Dir
set(BIN_DIR "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BIN_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BIN_DIR}")

add_subdirectory("Transit")