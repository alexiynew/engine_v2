include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)

FetchContent_MakeAvailable(glfw)

set_target_properties(glfw uninstall update_mappings PROPERTIES FOLDER "libs/glfw3")

# Define lids to link
add_library(libs::glfw INTERFACE IMPORTED)
target_link_libraries(libs::glfw INTERFACE glfw)
target_include_directories(libs::glfw INTERFACE ${glfw_SOURCE_DIR}/include)
