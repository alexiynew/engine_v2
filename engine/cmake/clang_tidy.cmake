# Custom target to run clang-tidy

find_program(CLANG_TIDY clang-tidy)
if(CLANG_TIDY)
    execute_process(COMMAND CLANG_TIDY --version OUTPUT_VARIABLE CLANG_TIDY_VERSION)  

    file(GLOB_RECURSE SOURCES 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/**.[ch]pp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/**.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/**.[ch]pp
        ${CMAKE_CURRENT_SOURCE_DIR}/entry_point/**.[ch]pp
        ${CMAKE_CURRENT_SOURCE_DIR}/modules/**.[ch]pp
    )

    add_custom_target(clang-tidy
        COMMAND ${CLANG_TIDY}   
            -p ${CMAKE_BINARY_DIR}
            --config-file ${CMAKE_SOURCE_DIR}/.clang-tidy
            ${SOURCES}
        COMMAND_EXPAND_LISTS
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT Run clang-tidy on source files
    )

    set_target_properties(clang-tidy PROPERTIES FOLDER "engine/utils")
else()
    message(WARNING "clang-tidy executable not found")
endif()