cmake_minimum_required(VERSION 3.27)

# Engine project setup
project(engine LANGUAGES CXX VERSION 0.0.1)

# Options
option(ENABLE_TESTING "Enable the building of the test" ON)

# TODO: Implement address sanitazer build
option(ENABLE_ASAN "Build with address sanitazer" OFF)

option(ENABLE_TRACY "Build with tracy profiler" OFF)

# TODO: Implement documentation generation
option(BUILD_DOCUMENTATION "Build documentation" OFF)

option(USE_STUB_BACKEND "Use backend stub for testing" OFF)
option(USE_STUB_RENDERER "Use renderer stub for testing" OFF)

if (WIN32)
    set(USE_WINMAIN_DEFAULT ON)
else ()
    set(USE_WINMAIN_DEFAULT OFF)
endif ()
option(USE_WINMAIN "Use WinMain entry point" ${USE_WINMAIN_DEFAULT})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Misc settings
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Choose backend type
set(ENGINE_BACKEND_TYPE glfw CACHE STRING "" FORCE)
if (USE_STUB_BACKEND)
    set(ENGINE_BACKEND_TYPE stub CACHE STRING "" FORCE)
endif ()

# Choose renderer type
set(ENGINE_RENDERER_TYPE opengl CACHE STRING "" FORCE)
if (USE_STUB_RENDERER)
    set(ENGINE_RENDERER_TYPE stub CACHE STRING "" FORCE)
endif ()

# Check backend and renderer
if (USE_STUB_BACKEND AND NOT USE_STUB_RENDERER)
    message(FATAL_ERROR "Can't use any renderer with stub backend")
endif ()

# Add modules path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Engine library
add_library(engine STATIC)

# Collect sources
file(GLOB_RECURSE ENGINE_HEADERS CONFIGURE_DEPENDS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "include/engine/**.hpp")
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/**.hpp" "src/**.cpp")

target_sources(engine
    PUBLIC
        ${ENGINE_HEADERS}
    PRIVATE
        ${ENGINE_SOURCES}
)

target_include_directories(engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Include engine parts
include(cmake/compile_options)
include(third_party/glm)

add_subdirectory(entry_point)
add_subdirectory(modules)

target_link_libraries(engine
    INTERFACE
        engine::entry_point

    PUBLIC
        third_party::glm

    PRIVATE
        engine::compile_options
        engine::modules
)

# Tracy
if (ENABLE_TRACY)
    include(third_party/tracy)

    target_link_libraries(engine
        PRIVATE
            third_party::tracy
    )
endif ()

# Tests
if (ENABLE_TESTING)
    enable_testing()
    include(tests/tests)
endif ()

# Custom tool targets
include(cmake/format_all)
include(cmake/cppcheck)
include(cmake/clang_tidy)

set_target_properties(engine PROPERTIES FOLDER "engine")
set_target_properties(engine PROPERTIES EXPORT_COMPILE_COMMANDS ON)
if (USE_WINMAIN)
    set_target_properties(engine PROPERTIES WIN32_EXECUTABLE ON)
endif ()

# Status
message(STATUS "#########################################")
message(STATUS "# Engine Summary")
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
message(STATUS "Clang-tidy:          \t ${CLANG_TIDY} ${CLANG_TIDY_VERSION}")
message(STATUS "Cppcheck:            \t ${CPPCHECK} ${CPPCHECK_VERSION}")
message(STATUS "Preset name:         \t ${BUILD_PRESET_NAME}")
message(STATUS "Build documentation: \t ${BUILD_DOCUMENTATION}")
message(STATUS "#########################################")
