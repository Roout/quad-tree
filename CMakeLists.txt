cmake_minimum_required(VERSION 3.17.0)

project(App)

# build quad tree library
add_subdirectory("src")
# build application
add_subdirectory("app")
