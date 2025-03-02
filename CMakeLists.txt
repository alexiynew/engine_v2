cmake_minimum_required(VERSION 3.27)


option(ENABLE_TESTING "Enable the building of the test" ON)
option(ENABLE_CPPCHECK "Enable code checking with cppcheck" ON)
option(USE_STUB_BACKEND "Use backend stub for testing" ON)

project(GameEngine LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Misc settings
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add modules path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Engine
add_executable(engine)

target_sources(engine
    PUBLIC
        include/game_engine/common_types.hpp
        include/game_engine/game_settings.hpp
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
        src/entry_point/entry_point.cpp
        src/game_settings.cpp
        src/game_stub.hpp
        src/game_stub.cpp
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
add_custom_target(format-all
    COMMAND ${CMAKE_COMMAND} -P  cmake/format_all.cmake
    COMMAND_EXPAND_LISTS
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    COMMENT Format all source files
)

set_target_properties(format-all PROPERTIES FOLDER "utils")

if(ENABLE_CPPCHECK)
    find_program(CPPCHECK cppcheck)
    if(CPPCHECK)
        add_custom_target(cppcheck
            COMMAND ${CMAKE_COMMAND} -E make_directory build/cppcheck
            COMMAND ${CPPCHECK}   
                    -q
                    --enable=all   
                    --project=build/compile_commands.json   
                    --template=gcc 
                    --std=c++20 
                    -ibuild
                    -j9
                    --inconclusive
                    --suppress=missingIncludeSystem:*
                    --suppress=*:build/*
                    --suppress=checkersReport
                    --checkers-report=build/cppcheck/checkers_report.txt
                    --cppcheck-build-dir=build/cppcheck
            COMMAND_EXPAND_LISTS
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            COMMENT Cppcheck source files
        )

        set_target_properties(cppcheck PROPERTIES FOLDER "utils")
    else()
        message(SEND_ERROR "cppcheck requested but executable not found")
    endif()
endif()

