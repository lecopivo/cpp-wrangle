# Specify the minimum required version of CMake to build the project.
cmake_minimum_required( VERSION 3.6 )

set(CMAKE_PREFIX_PATH "/opt/hfs17.0/toolkit/cmake")
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# Locate Houdini's libraries and header files.
# Registers an imported library target named 'Houdini'.
find_package(Houdini REQUIRED)
find_package(Eigen3 REQUIRED)
# Add a library with two source files.

add_library(CppWrangleExecutor SHARED CppWrangleExecutor.cpp)
# Link against the Houdini libraries, and add required include directories and compile definitions.

target_link_libraries(CppWrangleExecutor Houdini)

# target_include_directories(CppWrangleExecutor PUBLIC "${CMAKE_SOURCE_DIR}/.." ${EIGEN3_INCLUDE_DIR})
# Configure several common target properties, such as its output directory.
houdini_configure_target( CppWrangleExecutor )

#install (TARGETS cmake_callback DESTINATION lib)

add_subdirectory(wrangle)
