include(third_party/gtest)


set(ENGINE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(tests/engine)
add_subdirectory(tests/events)

add_subdirectory(tests/resource_management/loaders/obj_parser)
