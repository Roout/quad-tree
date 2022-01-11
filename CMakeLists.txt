cmake_minimum_required(VERSION 3.17.0)

project(App)

# build SFML
include(external/CMakeCache.txt)
add_subdirectory(external/SFML)
# build quad tree library
add_subdirectory("src")
# build application
add_subdirectory("app")
