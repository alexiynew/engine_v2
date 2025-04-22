include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.16.0
)

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

set_target_properties(gtest gtest_main gmock gmock_main PROPERTIES FOLDER "libs/gtest")

# Define lids to link
add_library(libs::gtest INTERFACE IMPORTED)
target_link_libraries(libs::gtest INTERFACE gtest_main)
target_include_directories(libs::gtest INTERFACE ${googletest_SOURCE_DIR}/googletest/include)

add_library(libs::gmock INTERFACE IMPORTED)
target_link_libraries(libs::gmock INTERFACE gmock_main)
target_include_directories(libs::gmock INTERFACE ${googletest_SOURCE_DIR}/googlemock/include)
