cmake_minimum_required(VERSION 3.28)
project(sim LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

find_package(EnTT REQUIRED)

add_executable(main 
    src/main.cpp
    src/simulation.cpp
    src/systems/renderer.cpp
    src/factories/creature_factory.cpp
    src/factories/plant_factory.cpp
    src/systems/movement_system.cpp
    src/systems/metabolism_system.cpp
    src/systems/cleanup_system.cpp
)

target_include_directories(main PRIVATE src/)
target_compile_features(main PRIVATE cxx_std_17)
target_compile_options(main PRIVATE -Wall -Wextra)
target_link_libraries(main PRIVATE SFML::Graphics EnTT::EnTT)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)