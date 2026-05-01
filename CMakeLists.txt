cmake_minimum_required(VERSION 3.28)
project(sim LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_BUILD_TYPE Release)
add_compile_options(-O2 -g)

# Dependencies
set(RAYLIB_VERSION 6.0)
find_package(raylib ${RAYLIB_VERSION} QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )

    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED)
        set(FETCHCONTENT_QUEIT NO)
        FetchContent_MakeAvailable(raylib)
    endif()
endif()

include(FetchContent)
FetchContent_Declare(EnTT
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.16.0)
FetchContent_MakeAvailable(EnTT)

add_executable(main 
    src/main.cpp
    src/simulation.cpp
    src/systems/renderer.cpp
    src/factories/creature_factory.cpp
    src/factories/plant_factory.cpp
    src/systems/movement_system.cpp
    src/systems/metabolism_system.cpp
    src/systems/death_system.cpp
    src/systems/collision_system.cpp
    src/systems/reproduction_system.cpp
    src/components/brain/brain.cpp
    src/components/brain/node.cpp
    src/systems/thinking_system.cpp
    src/factories/brain_factory.cpp
    src/systems/sensor_system.cpp
    src/random.cpp
    src/mutators/brain_mutator.cpp
    src/mutators/genome_mutator.cpp
    src/systems/plant_system.cpp
    src/quad_tree.cpp
    src/factories/carcass_factory.cpp
    src/systems/spatial_index_system.cpp
    src/systems/environment_system.cpp
    src/systems/digestion_system.cpp
    src/fertility.cpp
    profile.sh
)

target_include_directories(main PRIVATE src/ ${CMAKE_SOURCE_DIR}/include)
target_compile_features(main PRIVATE cxx_std_17)
target_compile_options(main PRIVATE -Wall -Wextra)
target_link_libraries(main PRIVATE SFML::Graphics EnTT::EnTT)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)