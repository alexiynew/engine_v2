find_program(CPPCHECK cppcheck)
if(CPPCHECK)
    add_custom_target(cppcheck
        COMMAND ${CMAKE_COMMAND} -E make_directory build/cppcheck
        COMMAND ${CPPCHECK}   
                -q
                --enable=all   
                --project=build/compile_commands.json   
                --template=gcc 
                --std=c++20 
                -ibuild
                -j9
                --inconclusive
                --suppress=missingIncludeSystem:*
                --suppress=*:build/*
                --suppress=checkersReport
                --checkers-report=build/cppcheck/checkers_report.txt
                --cppcheck-build-dir=build/cppcheck
        COMMAND_EXPAND_LISTS
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMENT Cppcheck source files
    )

    set_target_properties(cppcheck PROPERTIES FOLDER "utils")
else()
    message(SEND_ERROR "cppcheck requested but executable not found")
endif()