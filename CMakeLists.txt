cmake_minimum_required(VERSION 3.17.0)

set(This quad-tree)
project(${This} VERSION 0.1.0)


set(CMAKE_CXX_STANDARD 17)

set(SFML_DIR "external/SFML-build/")
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

set(sources
    "src/main.cpp"
    "src/Interface.cpp"
    "src/MainScene.cpp"
    "src/Node.cpp"
    "src/QuadTree.cpp"
    "src/QuadTreeLayout.cpp"
)

set(headers
    "src/healthy.h"
    "src/Graphics.h"
    "src/Interface.h"
    "src/MainScene.h"
    "src/Node.h"
    "src/QuadTree.h"
    "src/QuadTreeLayout.h"
)

add_executable(${This} ${sources} ${headers})

target_link_libraries(${This} sfml-graphics sfml-window sfml-system)

target_compile_options(${This} PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:$<$<CXX_COMPILER_ID:Clang>:-Wall -Werror -Wextra>>
    $<$<COMPILE_LANGUAGE:CXX>:$<$<CXX_COMPILER_ID:GNU>:-Wall -Werror -Wextra>>
    $<$<COMPILE_LANGUAGE:CXX>:$<$<CXX_COMPILER_ID:MSVC>:/W3>>
)

add_custom_command(
        TARGET ${This} POST_BUILD
        COMMAND echo "Copying assets..."
        COMMAND ${CMAKE_COMMAND} -E copy_directory   
                ${CMAKE_CURRENT_SOURCE_DIR}/assets
                $<TARGET_FILE_DIR:${This}>/assets
        COMMAND echo "Copying libs..."
        COMMAND ${CMAKE_COMMAND} -E copy_directory   
                ${CMAKE_CURRENT_SOURCE_DIR}/external/SFML-build/lib
                $<TARGET_FILE_DIR:${This}>
)