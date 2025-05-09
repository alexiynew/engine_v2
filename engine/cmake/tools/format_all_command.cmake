
file(GLOB_RECURSE SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.[ch]pp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/**/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/**/*.[ch]pp
    ${CMAKE_CURRENT_SOURCE_DIR}/entry_point/**/*.[ch]pp
)

# string(JOIN "\n" SOURCES_STRING ${SOURCES})
# message(STATUS "\n${SOURCES_STRING}")

execute_process(
    COMMAND clang-format -i -style=file ${SOURCES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)