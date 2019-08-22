# Specify the minimum required version of CMake to build the project.
cmake_minimum_required( VERSION 3.6 )

set(CMAKE_PREFIX_PATH "${HOUDINI_ROOT}/toolkit")

### LIBRARIES ###
find_package(Houdini REQUIRED)
find_package(Eigen3 REQUIRED)

### EXECUTOR ###

# SOP
add_library(CppWrangleExecutor SHARED CppWrangleExecutor.cpp)
target_link_libraries(CppWrangleExecutor Houdini)
houdini_configure_target( CppWrangleExecutor )

# DOP
add_library(CppDOPExecutor SHARED CppDOPExecutor.cpp)
target_link_libraries(CppDOPExecutor Houdini)
houdini_configure_target( CppDOPExecutor )

# # DOP
# add_library(AutoDOP SHARED AutoDOP.cpp)
# target_link_libraries(AutoDOP Houdini)
# houdini_configure_target( AutoDOP )


### WRANGLES ###
add_subdirectory(cppvex)
add_subdirectory(houfem)
add_subdirectory(wrangle)
