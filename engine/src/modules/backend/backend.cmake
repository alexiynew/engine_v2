# Check backend type
set(SUPPORTED_BACKENDS glfw stub)

if(NOT ENGINE_BACKEND_TYPE IN_LIST SUPPORTED_BACKENDS)
    message(FATAL_ERROR "Backend ${ENGINE_BACKEND_TYPE} isn't supported.")
endif()

# Backend library
add_subdirectory(src/modules/backend/${ENGINE_BACKEND_TYPE})
