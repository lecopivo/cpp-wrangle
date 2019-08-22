file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(cppvex ${SRC_FILES})

set_property(TARGET cppvex PROPERTY CXX_STANDARD 17)
target_compile_options(cppvex PUBLIC "-fPIC")
target_include_directories(cppvex SYSTEM PUBLIC ${EIGEN3_INCLUDE_DIR})
target_link_libraries(cppvex Houdini)

