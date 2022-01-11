cmake_minimum_required(VERSION 3.17.0)

set(This visualization)
project(${This} VERSION 0.1.0)


set(CMAKE_CXX_STANDARD 17)

set(QUADTREE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src")

set(sources
    "main.cpp"
    "Interface.cpp"
    "MainScene.cpp"
    "Node.cpp"
    "QuadTreeLayout.cpp"
)

set(headers
    "Graphics.h"
    "Interface.h"
    "MainScene.h"
    "Node.h"
    "QuadTreeLayout.h"
)

add_executable(${This} ${sources} ${headers})

target_include_directories(${This} PRIVATE ${QUADTREE_INCLUDE_DIR})

target_link_libraries(${This} PRIVATE sfml-graphics sfml-window sfml-system qtreelib)

target_compile_options(${This} PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:$<$<CXX_COMPILER_ID:Clang>:-Wall -Werror -Wextra -pedantic>>
    $<$<COMPILE_LANGUAGE:CXX>:$<$<CXX_COMPILER_ID:GNU>:-Wall -Werror -Wextra -pedantic>>
    $<$<COMPILE_LANGUAGE:CXX>:$<$<CXX_COMPILER_ID:MSVC>:/W3>>
)

add_custom_command(
    TARGET ${This} POST_BUILD
    COMMAND echo "Copying assets..."
    COMMAND ${CMAKE_COMMAND} -E copy_directory   
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:${This}>/assets
    COMMAND echo "Copying shared libs..."
    COMMAND ${CMAKE_COMMAND} -E copy_directory   
            ${CMAKE_BINARY_DIR}/external/SFML/lib
            $<TARGET_FILE_DIR:${This}>
)