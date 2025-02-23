
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/**/*.[ch]pp" "include/**/*.hpp" "tests/**/*.[ch]pp")

execute_process(COMMAND clang-format -i -style=file ${SOURCES})
