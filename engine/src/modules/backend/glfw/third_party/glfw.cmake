include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)

FetchContent_MakeAvailable(glfw)

set_target_properties(glfw uninstall update_mappings PROPERTIES FOLDER "engine/third_party/glfw3")

# Define lids to link
add_library(third_party::glfw INTERFACE IMPORTED)
target_link_libraries(third_party::glfw INTERFACE glfw)
target_include_directories(third_party::glfw INTERFACE ${glfw_SOURCE_DIR}/include)
