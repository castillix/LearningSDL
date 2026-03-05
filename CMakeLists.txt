cmake_minimum_required(VERSION 3.16)
project(Transit VERSION 0.1.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Dependencies
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# Set Output Dir
set(BIN_DIR "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>-x64")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BIN_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BIN_DIR}")

# Engine Core Library
file(GLOB_RECURSE ENGINE_SRC_FILES "Engine/*.cpp") 
add_library(Engine STATIC ${ENGINE_SRC_FILES})
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Engine PUBLIC glm::glm)
target_link_libraries(Engine PRIVATE 
    SDL3::SDL3
    $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>
    glad::glad
    imgui::imgui
)

# Zombies Target
file(GLOB_RECURSE ZOMBIES_SRC_FILES "Zombies/*.cpp")
add_executable(Zombies ${ZOMBIES_SRC_FILES})
target_link_libraries(Zombies PRIVATE Engine)
add_custom_command(TARGET Zombies POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Zombies/assets"
        "$<TARGET_FILE_DIR:Zombies>/assets"
)
