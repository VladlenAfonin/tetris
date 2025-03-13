add_executable(${PROJECT_NAME} main.c grid.c shape.c colors.c globals.c)

target_compile_options(
  ${PROJECT_NAME}
  PRIVATE -Wall
  PRIVATE -Wextra)
target_include_directories(${PROJECT_NAME} PRIVATE ../ext/raylib/src)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
