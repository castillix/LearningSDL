cmake_minimum_required(VERSION 3.16)
project(Transit VERSION 0.1.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Dependencies
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)

# Set Output Dir
set(BIN_DIR "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>-x64")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BIN_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BIN_DIR}")

file(GLOB SRC_FILES "Snake/*.cpp")
add_executable(Transit ${SRC_FILES})

target_link_libraries(Transit PRIVATE 
    SDL3::SDL3
    $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>
)

add_custom_command(TARGET Transit POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Project/assets"
        "$<TARGET_FILE_DIR:Transit>/assets"
)