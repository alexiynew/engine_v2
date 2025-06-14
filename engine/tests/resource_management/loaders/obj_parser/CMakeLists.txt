add_executable(obj_parser_test)

target_sources(obj_parser_test
    PRIVATE
        main.cpp
        tests_obj_parser.cpp
)

target_include_directories(obj_parser_test
    PRIVATE
        $<TARGET_PROPERTY:engine,INTERFACE_INCLUDE_DIRECTORIES>
        ${ENGINE_SOURCE_DIR}
)

target_link_libraries(obj_parser_test
    PRIVATE
        engine
        engine::compile_options
        third_party::gtest
        third_party::gmock
)

set_target_properties(obj_parser_test PROPERTIES FOLDER "engine/tests/resource_management/loaders/obj_parser")

add_test(NAME obj_parser_test COMMAND obj_parser_test)
