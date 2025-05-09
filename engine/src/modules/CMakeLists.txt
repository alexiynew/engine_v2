# Library to include modules interfaces
add_library(modules INTERFACE)

target_include_directories(modules
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

include(backend/backend.cmake)
include(graphics/graphics.cmake)

target_link_libraries(modules 
    INTERFACE
    engine::modules::backend
    engine::modules::graphics
)

set_target_properties(modules PROPERTIES FOLDER "engine/modules")
add_library(engine::modules ALIAS modules)
