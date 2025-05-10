add_executable(engine_test)

target_sources(engine_test
    PRIVATE
        main.cpp
        event_system.cpp
)

target_include_directories(engine_test
    PRIVATE 
        $<TARGET_PROPERTY:engine,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(engine_test
    PRIVATE 
        engine
        engine::compile_options
        third_party::gtest 
        third_party::gmock
)

set_target_properties(engine_test PROPERTIES FOLDER "engine/tests")

add_test(NAME engine_test COMMAND engine_test)
