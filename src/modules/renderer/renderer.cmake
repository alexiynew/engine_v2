# Check renderer type
set(SUPPORTED_RENDERERS opengl stub)

if(NOT ENGINE_RENDERER_TYPE IN_LIST SUPPORTED_RENDERERS)
    message(FATAL_ERROR "Renderer ${ENGINE_RENDERER_TYPE} isn't supported.")
endif()

# Renderer library
add_subdirectory(src/modules/renderer/${ENGINE_RENDERER_TYPE})
