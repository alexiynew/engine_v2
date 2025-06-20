include(FetchContent)

FetchContent_Declare(
    tracy
    GIT_REPOSITORY https://github.com/wolfpld/tracy.git
    GIT_TAG        v0.12.1
)

set(TRACY_ENABLE ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(tracy)

set_target_properties(TracyClient PROPERTIES FOLDER "engine/third_party/tracy")

# Define lids to link
add_library(third_party::tracy INTERFACE IMPORTED)
target_link_libraries(third_party::tracy INTERFACE TracyClient)
target_include_directories(third_party::tracy INTERFACE ${tracy_SOURCE_DIR}/public)
