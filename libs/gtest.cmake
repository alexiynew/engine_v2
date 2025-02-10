include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
)

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

set(GTEST_CUSTOM_GCC_OPTIONS -Wno-undef)

add_library(gtest_custom_compile_options INTERFACE)
target_compile_options(gtest_custom_compile_options 
    INTERFACE 
        $<${GCC_LIKE_COMPILER}:${GTEST_CUSTOM_GCC_OPTIONS}>
        $<${MSVC_LIKE_COMPILER}:${GTEST_CUSTOM_MSVC_OPTIONS}>
)

FetchContent_MakeAvailable(googletest)

target_link_libraries(gtest PRIVATE gtest_custom_compile_options)
target_link_libraries(gtest_main PRIVATE gtest_custom_compile_options)
target_link_libraries(gmock PRIVATE gtest_custom_compile_options)
target_link_libraries(gmock_main PRIVATE gtest_custom_compile_options)

set_target_properties(gtest gtest_main gmock gmock_main PROPERTIES FOLDER "libs/gtest")

# Define lids to link
add_library(Libs::GTest INTERFACE IMPORTED)
target_link_libraries(Libs::GTest INTERFACE gtest_main)
target_include_directories(Libs::GTest INTERFACE ${googletest_SOURCE_DIR}/googletest/include)

add_library(Libs::GMock INTERFACE IMPORTED)
target_link_libraries(Libs::GMock INTERFACE gmock_main)
target_include_directories(Libs::GMock INTERFACE ${googletest_SOURCE_DIR}/googletest/include)
