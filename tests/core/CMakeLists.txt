cmake_path(GET CMAKE_CURRENT_LIST_DIR STEM LAST_ONLY DIR_NAME)
set(TEST_NAME ${DIR_NAME}_test)

add_executable(${TEST_NAME} main.cpp)

target_include_directories(${TEST_NAME} 
    PRIVATE 
        ${CMAKE_SOURCE_DIR}/include 
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${TEST_NAME} 
    PRIVATE 
        Libs::gtest 
        Libs::gmock
)

set_target_properties(${TEST_NAME} PROPERTIES FOLDER "tests")

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
