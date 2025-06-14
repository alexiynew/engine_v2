add_executable(engine_test)

target_sources(engine_test
    PRIVATE
        main.cpp
        tests_engine_impl.cpp
)

target_include_directories(engine_test
    PRIVATE
        $<TARGET_PROPERTY:engine,INTERFACE_INCLUDE_DIRECTORIES>
        ${ENGINE_SOURCE_DIR}
        ../../modules/include
)

target_link_libraries(engine_test
    PRIVATE
        engine
        engine::compile_options
        third_party::gtest
        third_party::gmock
)

set_target_properties(engine_test PROPERTIES FOLDER "engine/tests/engine")

add_test(NAME engine_test COMMAND engine_test)
