cmake_minimum_required(VERSION 3.28)
project(MyGame LANGUAGES CXX)



include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.1
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(untitled13 main.cpp)
target_compile_features(untitled13 PRIVATE cxx_std_17)
target_link_libraries(untitled13 PRIVATE SFML::Graphics)