add_library(houfem simplex_gm_rule.cpp)

set_property(TARGET houfem PROPERTY CXX_STANDARD 17)
target_compile_options(houfem PUBLIC "-fPIC")
target_include_directories(houfem SYSTEM PUBLIC ${EIGEN3_INCLUDE_DIR})
target_link_libraries(houfem cppvex Houdini)
