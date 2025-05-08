# Custom target to format all sources

add_custom_target(format-all
    COMMAND ${CMAKE_COMMAND} -P cmake/tools/format_all_command.cmake
    COMMAND_EXPAND_LISTS
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    COMMENT Format all source files
)

set_target_properties(format-all PROPERTIES FOLDER "engine/utils")
