
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/**/*.[ch]pp" "include/*.hpp")

execute_process(COMMAND clang-format -i -style=file ${SOURCES})
