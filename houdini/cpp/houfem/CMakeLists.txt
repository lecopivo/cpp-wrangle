cmake_minimum_required(VERSION 2.8.11)

list( APPEND CMAKE_PREFIX_PATH "~/houdini17/toolkit/cmake" )

find_package(Eigen3 REQUIRED)
find_package(Houdini REQUIRED)

add_compile_options(-std=c++17 -fPIC -Wno-ignored-attributes -Wno-comment)

# Simplex Library 
add_library(houfem simplex_gm_rule.cpp)
target_include_directories(houfem SYSTEM PUBLIC ${EIGEN3_INCLUDE_DIR})

# Test Executable
add_executable(main main.cpp)
target_link_libraries(main houfem Houdini pthread)

# Installation
file(GLOB_RECURSE HDRS *.h)
install(TARGETS houfem DESTINATION lib)
install(FILES ${HDRS} DESTINATION include)
