cmake_minimum_required(VERSION 3.0.0)

set(This quad-tree)
project(${This} VERSION 0.1.0)


set(CMAKE_CXX_STANDARD 17)

set(SFML_DIR "../vendor/SFML/")
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

set(sources
    "main.cpp"
    "Interface.cpp"
    "MainScene.cpp"
    "Node.cpp"
    "QuadTree.cpp"
    "QuadTreeLayout.cpp"
)

set(headers
    "healthy.h"
    "Graphics.h"
    "Interface.h"
    "MainScene.h"
    "Node.h"
    "QuadTree.h"
    "QuadTreeLayout.h"
)

add_executable(${This} ${sources} ${headers})

target_link_libraries(${This} sfml-graphics sfml-window sfml-system)