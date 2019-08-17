# Add a library with two source files.

set(EXTRA_CXX_FLAGS "-Wno-comment;-Wno-ignored-attributes;-Wno-misleading-indentation;-Wno-unused-variable;-Wno-int-in-bool-context")

file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

foreach(SRC ${SRC_FILES})
  message(${SRC})
  get_filename_component(NAME ${SRC} NAME_WE)
  message(${NAME})

  add_library(${NAME} SHARED ${SRC})

  set_property(TARGET ${NAME} PROPERTY CXX_STANDARD 17)
  target_compile_options(${NAME} PUBLIC ${EXTRA_CXX_FLAGS})
  target_include_directories(${NAME} PUBLIC "${CMAKE_SOURCE_DIR}" ${EIGEN3_INCLUDE_DIR})
  target_link_libraries(${NAME} Houdini houfem)

  install (TARGETS ${NAME} DESTINATION lib)
endforeach()

