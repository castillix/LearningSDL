file(GLOB_RECURSE TRANSIT_SOURCES "src/*.cpp") # probably bad practice ╓⌈፡↊
add_library(Transit SHARED ${TRANSIT_SOURCES})

target_precompile_headers(Transit PRIVATE ./include/Core.h)

# Definitions 
target_compile_definitions(Transit 
    PRIVATE 
        TRANSIT_BUILD_DLL        # Only the engine builds the DLL
    PUBLIC 
        TRANSIT_PLATFORM_WINDOWS
)

target_include_directories(Transit 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE 
        src        # Only Transit can see src
)

# Link Extra Libraries
target_link_libraries(Transit PUBLIC 
    SDL3::SDL3 
    SDL3_image::SDL3_image
)