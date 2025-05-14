# Library to include modules interfaces
add_library(modules INTERFACE)

target_include_directories(modules
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(modules 
    INTERFACE
        include/modules/module_factory.hpp
        include/modules/module_registrar.hpp
        include/modules/input_handler.hpp
        include/modules/render_context.hpp
        include/modules/backend/backend.hpp
        include/modules/graphics/renderer.hpp
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
