# Specify the minimum required version of CMake to build the project.
cmake_minimum_required( VERSION 3.6 )

set(CMAKE_PREFIX_PATH "${HOUDINI_ROOT}/toolkit")

### LIBRARIES ###
find_package(Houdini REQUIRED)
find_package(Eigen3 REQUIRED)

### EXECUTOR ###

add_library(CppWrangleExecutor SHARED CppWrangleExecutor.cpp)
target_link_libraries(CppWrangleExecutor Houdini)
houdini_configure_target( CppWrangleExecutor )

### WRANGLES ###
add_subdirectory(houfem)
add_subdirectory(wrangle)
