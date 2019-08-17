# Add a library with two source files.

file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

message("ZDROJAKY JSOU: ${SRC_FILES}")


foreach(SRC ${SRC_FILES})
  message(${SRC})
  get_filename_component(NAME ${SRC} NAME_WE)
  message(${NAME})

  add_library(${NAME} SHARED ${SRC})
  
  target_link_libraries(${NAME} Houdini)
  target_include_directories(${NAME} PUBLIC "${CMAKE_SOURCE_DIR}" ${EIGEN3_INCLUDE_DIR})

  install (TARGETS ${NAME} DESTINATION lib)
endforeach()

