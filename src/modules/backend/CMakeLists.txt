# Check backend type
set(SUPPORTED_BACKENDS glfw stub)

if(NOT ENGINE_BACKEND_TYPE IN_LIST SUPPORTED_BACKENDS)
    message(FATAL_ERROR "Backend ${ENGINE_BACKEND_TYPE} isn't supported.")
endif()

# Backend library
add_library(backend STATIC)

target_sources(backend
    PUBLIC
        include/backend/backend.hpp
    PRIVATE
        src/backend.cpp
)

target_include_directories(backend 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(backend PROPERTIES FOLDER "backend")

include(src/${ENGINE_BACKEND_TYPE}/CMakeLists.txt)
include(libs/glm)

target_link_libraries(backend
    PRIVATE 
        engine::compile_options
        libs::glm
)
