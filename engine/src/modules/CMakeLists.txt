# Library to include modules interfaces
add_library(modules INTERFACE)

target_include_directories(modules
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(modules PROPERTIES FOLDER "engine/modules")

add_library(engine::modules ALIAS modules)
