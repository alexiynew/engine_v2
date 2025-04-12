# Backend library

set(SUPPORTED_BACKENDS glfw stub)

if(NOT ENGINE_BACKEND_TYPE IN_LIST SUPPORTED_BACKENDS)
    message(FATAL_ERROR "Backend ${ENGINE_BACKEND_TYPE} isn't supported.")
endif()

add_library(backend STATIC)

target_sources(backend
    PUBLIC
        backend.hpp
    PRIVATE
        backend.cpp
)

target_include_directories(backend 
    PRIVATE 
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(backend PROPERTIES FOLDER "backend")

include(${ENGINE_BACKEND_TYPE}/CMakeLists.txt)

include(libs/glm)
target_link_libraries(backend PUBLIC Libs::glm)

target_link_libraries(backend PRIVATE engine::compile_options)

add_library(engine::backend ALIAS backend)
