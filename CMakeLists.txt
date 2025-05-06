cmake_minimum_required(VERSION 3.27)

include(CMakeDependentOption)

# Options
option(ENABLE_TESTING "Enable the building of the test" ON)

# TODO: Implement address sanitazer build
option(ENABLE_ASAN "Build with address sanitazer" OFF)

# TODO: Implement documentation generation
option(BUILD_DOCUMENTATION "Build documentation" OFF)

option(USE_STUB_BACKEND "Use backend stub for testing" OFF)
option(USE_STUB_RENDERER "Use renderer stub for testing" OFF)

# Use WinMain on windows as entry point
cmake_dependent_option(USE_WINMAIN "Use WinMain function as entry point instead of main (window application)" ON WIN32 OFF) 


# Main project setup
project(GameEngine LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Misc settings
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Choose backend type
set(ENGINE_BACKEND_TYPE glfw CACHE STRING "" FORCE)
if(USE_STUB_BACKEND)
    set(ENGINE_BACKEND_TYPE stub CACHE STRING "" FORCE)
endif()

# Choose renderer type
set(ENGINE_RENDERER_TYPE opengl CACHE STRING "" FORCE)
if(USE_STUB_RENDERER)
    set(ENGINE_RENDERER_TYPE stub CACHE STRING "" FORCE)
endif()

# Check backend and renderer
if(USE_STUB_BACKEND AND NOT USE_STUB_RENDERER)
    message(FATAL_ERROR "Can't use any renderer with stub backend")
endif()

# Add modules path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Engine
add_executable(engine)

target_sources(engine
    PUBLIC
        include/game_engine/common_types.hpp
        include/game_engine/core/engine.hpp
        include/game_engine/core/mesh_data.hpp
        include/game_engine/core/mesh.hpp
        include/game_engine/core/model_loader.hpp
        include/game_engine/core/shader.hpp
        include/game_engine/core/vertex_traits.hpp
        include/game_engine/game_settings.hpp
        include/game_engine/game.hpp
        include/game_engine/keyboard.hpp
    PRIVATE
        src/modules/include/modules/module_factory.hpp
        src/modules/include/modules/backend.hpp
        src/modules/include/modules/renderer.hpp
        src/core/engine_impl.cpp
        src/core/engine_impl.hpp
        src/core/mesh_impl.cpp
        src/core/model_loader.cpp
        src/entry_point/entry_point.cpp
        src/game_settings.cpp
)

target_include_directories(engine 
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/include
    PRIVATE 
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/modules/include
)

# Compile options
include(cmake/compile_options)
target_link_libraries(engine PRIVATE engine::compile_options)

# Properties and definitions
if (USE_WINMAIN)
    set_target_properties(engine PROPERTIES WIN32_EXECUTABLE ON)
    target_compile_definitions(engine PRIVATE USE_WINMAIN)
endif()

# Backend
include(src/modules/backend/backend)
target_link_libraries(engine PRIVATE modules::backend)

# Renderer
include(src/modules/renderer/renderer)
target_link_libraries(engine PRIVATE modules::renderer)

# GLM
include(third_party/glm)
target_link_libraries(engine PUBLIC third_party::glm)

# Game
add_subdirectory(game)
target_link_libraries(engine PRIVATE game)

# Tests
if(ENABLE_TESTING)
    enable_testing()
    include(tests/CMakeLists.txt)
endif()

# Custom tool targets
include(cmake/format_all)
include(cmake/cppcheck)
include(cmake/clang_tidy)

# Status
message(STATUS "#########################################")
message(STATUS "\t \t Summary")
message(STATUS "#########################################")
message(STATUS "Version:             \t ${PROJECT_VERSION}")
message(STATUS "-----------------------------------------")
message(STATUS "System:              \t ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION}")
message(STATUS "Backend type:        \t ${ENGINE_BACKEND_TYPE}")
message(STATUS "Renderer type:       \t ${ENGINE_RENDERER_TYPE}")
message(STATUS "Use WinMain:         \t ${USE_WINMAIN}")
message(STATUS "Build type:          \t ${CMAKE_BUILD_TYPE}")
message(STATUS "Install prefix:      \t ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Compiler:            \t ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Testing enabled:     \t ${ENABLE_TESTING}")
message(STATUS "Address sanitizer:   \t ${ENABLE_ASAN}")
message(STATUS "Clang-tidy:          \t ${CLANG_TIDY}")
message(STATUS "Cppcheck:            \t ${CPPCHECK}")
message(STATUS "Preset name:         \t ${BUILD_PRESET_NAME}")
message(STATUS "Build documentation: \t ${BUILD_DOCUMENTATION}")
message(STATUS "#########################################")
