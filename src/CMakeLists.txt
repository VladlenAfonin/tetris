add_executable(${PROJECT_NAME} main.c grid.c shape.c colors.c)

target_include_directories(${PROJECT_NAME} PRIVATE ../ext/raylib/src)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
