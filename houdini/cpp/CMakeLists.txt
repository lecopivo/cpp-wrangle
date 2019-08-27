# Specify the minimum required version of CMake to build the project.
cmake_minimum_required( VERSION 3.6 )

set(CMAKE_PREFIX_PATH "${HOUDINI_ROOT}/toolkit")

### LIBRARIES ###
find_package(Houdini REQUIRED)
find_package(Eigen3 REQUIRED)

### REMOVE PESKY WARRNINGS ###

set(EXTRA_CXX_FLAGS "-Wno-comment;-Wno-ignored-attributes;-Wno-misleading-indentation;-Wno-unused-variable;-Wno-int-in-bool-context")

### EXECUTOR ###

# SOP
add_library(CppWrangle SHARED CppWrangleExecutor.cpp CppDOPExecutor.cpp LoadPlugin.cpp RegisteredObjects.cpp)
set_property(TARGET CppWrangle PROPERTY CXX_STANDARD 17)
target_compile_options(CppWrangle PUBLIC ${EXTRA_CXX_FLAGS})
target_link_libraries(CppWrangle Houdini)
target_include_directories(CppWrangle PRIVATE ${EIGEN3_INCLUDE_DIR})
houdini_configure_target( CppWrangle )

### WRANGLES ###
add_subdirectory(cppvex)
add_subdirectory(houfem)
add_subdirectory(wrangle)
