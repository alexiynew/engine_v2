add_executable(core_test)

target_sources(core_test
    PRIVATE
        main.cpp
)

target_include_directories(core_test
    PRIVATE 
        $<TARGET_PROPERTY:engine,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(core_test
    PRIVATE 
        engine::compile_options
        third_party::gtest 
        third_party::gmock
)

set_target_properties(core_test PROPERTIES FOLDER "engine/tests")

add_test(NAME core_test COMMAND core_test)
set_tests_properties(core_test
    PROPERTIES TIMEOUT 2
)
