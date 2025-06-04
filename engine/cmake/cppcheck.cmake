# Custom target to run cppcheck

find_program(CPPCHECK cppcheck)
if(CPPCHECK)
    execute_process(COMMAND cppcheck --version OUTPUT_VARIABLE CPPCHECK_VERSION)  
    
    add_custom_target(cppcheck
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/build/cppcheck
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
                --check-level=exhaustive
                --checkers-report=${CMAKE_SOURCE_DIR}/build/cppcheck/checkers_report.txt
                --cppcheck-build-dir=${CMAKE_SOURCE_DIR}/build/cppcheck
        COMMAND_EXPAND_LISTS
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT Cppcheck source files
    )

    set_target_properties(cppcheck PROPERTIES FOLDER "engine/utils")
else()
    message(WARNING "cppcheck executable not found")
endif()