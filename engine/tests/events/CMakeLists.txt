add_executable(events_test)

target_sources(events_test
    PRIVATE
        main.cpp
        tests_event_system.cpp
)

target_include_directories(events_test
    PRIVATE
        $<TARGET_PROPERTY:engine,INTERFACE_INCLUDE_DIRECTORIES>
        ../../src
)

target_link_libraries(events_test
    PRIVATE
        engine
        engine::compile_options
        third_party::gtest
        third_party::gmock
)

set_target_properties(events_test PROPERTIES FOLDER "engine/tests/events")

add_test(NAME events_test COMMAND events_test)
