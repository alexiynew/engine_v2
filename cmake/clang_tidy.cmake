find_program(CLANG_TIDY clang-tidy)
if(CLANG_TIDY)
    file(GLOB FILES ${CMAKE_SOURCE_DIR}/include/**/*.hpp
                    ${CMAKE_SOURCE_DIR}/src/**/*.[ch]pp
                    ${CMAKE_SOURCE_DIR}/tests/**/*.[ch]pp
    )

    set(CHECKS_LIST
        clang-analyzer-*
        modernize-*
        bugprone-*
        misc-const-correctness
        performance-*
        cppcoreguidelines-*
        -modernize-use-trailing-return-type
    )
    string(JOIN "," CHECKS ${CHECKS_LIST})

    add_custom_target(clang-tidy
        COMMAND ${CLANG_TIDY}   
            -checks="${CHECKS}"
            -p ${CMAKE_BINARY_DIR}
            -header-filter="\"^.*(include|src|tests).*\""
            ${FILES}
        COMMAND_EXPAND_LISTS
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMENT Run clang-tidy on source files
    )

    set_target_properties(clang-tidy PROPERTIES FOLDER "utils")
else()
    message(WARNING "clang-tidy executable not found")
endif()