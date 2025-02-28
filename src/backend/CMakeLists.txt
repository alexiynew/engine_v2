# Backend library
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


if (USE_STUB_BACKEND) 
    include(stub/CMakeLists.txt)
else()
    include(glfw/CMakeLists.txt)
endif()

include(libs/glm)
target_link_libraries(backend PUBLIC Libs::glm)

target_link_libraries(backend PRIVATE engine::compile_options)

add_library(engine::backend ALIAS backend)
