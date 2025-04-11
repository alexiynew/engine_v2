cmake_minimum_required(VERSION 3.27)

# Options
option(ENABLE_TESTING "Enable the building of the test" ON)

# TODO: Implement address sanitazer build
option(ENABLE_ASAN "Build with address sanitazer" OFF)

option(ENABLE_CPPCHECK "Enable code checking with cppcheck" ON)

# TODO: Implement clang-tidy build
option(ENABLE_CLANG_TIDY "Enable code checking with clang-tidy" OFF)

# TODO: Implement documentation generation
option(BUILD_DOCUMENTATION "Build documentation" OFF)

option(USE_STUB_BACKEND "Use backend stub for testing" OFF)

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
    message(++++++++++++++++++++++++++)
    set(ENGINE_BACKEND_TYPE stub CACHE STRING "" FORCE)
endif()

# Add modules path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Engine
add_executable(engine)

target_sources(engine
    PUBLIC
        include/game_engine/common_types.hpp
        include/game_engine/game.hpp
        include/game_engine/keyboard.hpp
        include/game_engine/core/engine.hpp
        include/game_engine/core/mesh.hpp
        include/game_engine/core/model_loader.hpp
    PRIVATE
        src/core/engine_impl.hpp
        src/core/engine_impl.cpp
        src/core/mesh.cpp
        src/core/model_loader.cpp
        src/game_stub.hpp
        src/game_stub.cpp
        src/entry_point/entry_point.cpp
)

target_include_directories(engine PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(engine PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Compile options
include(cmake/compile_options)
target_link_libraries(engine PRIVATE engine::compile_options)

# Backend
add_subdirectory(src/backend)
target_link_libraries(engine PRIVATE engine::backend)

# GLM
include(libs/glm)
target_link_libraries(engine PUBLIC Libs::glm)

# Tests
if(ENABLE_TESTING)
    enable_testing()
    include(tests/CMakeLists.txt)
endif()

# Custom target to format all sources
include (cmake/format_all)

if(ENABLE_CPPCHECK)
    include (cmake/cppcheck)
endif()


# Status
message(STATUS "#########################################")
message(STATUS "\t \t Summary")
message(STATUS "#########################################")
message(STATUS "Version:             \t ${PROJECT_VERSION}")
message(STATUS "-----------------------------------------")
message(STATUS "Backend type:        \t ${ENGINE_BACKEND_TYPE}")
message(STATUS "Build type:          \t ${CMAKE_BUILD_TYPE}")
message(STATUS "Install prefix:      \t ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Compiler:            \t ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Testing enabled:     \t ${ENABLE_TESTING}")
message(STATUS "Sanitizers:          \t ${ENABLE_ASAN}")
message(STATUS "Clang-tidy:          \t ${ENABLE_CLANG_TIDY} ${CLANGTIDY}")
message(STATUS "Cppcheck:            \t ${ENABLE_CPPCHECK} ${CPPCHECK}")
message(STATUS "Preset name:         \t ${BUILD_PRESET_NAME}")
message(STATUS "Build documentation: \t ${BUILD_DOCUMENTATION}")
message(STATUS "#########################################")
