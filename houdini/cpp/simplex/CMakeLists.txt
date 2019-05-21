cmake_minimum_required(VERSION 2.8.11)

add_compile_options(-std=c++17 -fPIC -Wignored-attributes)
find_package(Eigen3 REQUIRED)

# Simplex Library 
add_library(simplex simplex_gm_rule.cpp)
target_include_directories(simplex SYSTEM PUBLIC ${EIGEN3_INCLUDE_DIR})

# Test Executable
add_executable(main main.cpp)
target_link_libraries(main simplex)

# Installation
file(GLOB_RECURSE HDRS *.h)
install(TARGETS simplex DESTINATION lib)
install(FILES ${HDRS} DESTINATION include)
