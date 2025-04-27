find_program(CPPCHECK cppcheck)
if(CPPCHECK)
    add_custom_target(cppcheck
        COMMAND ${CMAKE_COMMAND} -E make_directory build/cppcheck
        COMMAND ${CPPCHECK}   
                -q
                --enable=all   
                --project=${CMAKE_BINARY_DIR}/compile_commands.json   
                --template=gcc 
                --std=c++20 
                -i_deps
                -j9
                --inconclusive
                --suppress=missingIncludeSystem:*
                --suppress=checkersReport
                --checkers-report=build/cppcheck/checkers_report.txt
                --cppcheck-build-dir=build/cppcheck
        COMMAND_EXPAND_LISTS
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMENT Cppcheck source files
    )

    set_target_properties(cppcheck PROPERTIES FOLDER "utils")
else()
    message(WARNING "cppcheck executable not found")
endif()