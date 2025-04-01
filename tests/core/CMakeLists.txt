add_executable(core_test)

target_sources(core_test
    PRIVATE
        main.cpp
        ${CMAKE_SOURCE_DIR}/src/backend/backend.cpp
        ${CMAKE_SOURCE_DIR}/src/core/engine_impl.cpp
        ${CMAKE_SOURCE_DIR}/src/core/mesh_impl.cpp
        ${CMAKE_SOURCE_DIR}/src/game_settings.cpp
        ${CMAKE_SOURCE_DIR}/src/game_stub.cpp
)

target_include_directories(core_test
    PRIVATE 
        ${CMAKE_SOURCE_DIR}/include 
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(core_test
    PRIVATE 
        Libs::gtest 
        Libs::gmock
        Libs::glm
)

target_link_libraries(core_test PRIVATE engine::compile_options)

set_target_properties(core_test PROPERTIES FOLDER "tests")

add_test(NAME core_test COMMAND core_test)
set_tests_properties(core_test
    PROPERTIES TIMEOUT 2
)
